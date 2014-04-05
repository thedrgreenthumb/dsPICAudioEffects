/**********************************************************************
* FileName:        Effects.c
* FileVersion      1.01
* Processor:       dsPIC33FJ128GP802
* Compiler:        C30 v3.21
*
* Design in:       SAL
* Design by:
* Feedback:
*
* Project:         dsPIC33 Audio Effects Pedal project.
* Project version: 1.01
**********************************************************************/

#include <libq.h>

//Matlab
#include "matlab/@fsaver/fsaver.h"
//Declare fsaver functions for Q16
fsaver_plane_data_declare(_Q16)
fsaver_iir_declare(_Q16)

#include "foundation.h"
#include "runner/runner.h"
#include "effects.h"

error_t bypass_init(bypass *bp, p_buffer_t buf)
{
    return ERROR_OK;
}
error_t bypass_set_params(void* dat, unsigned int num, unsigned int val)
{
    return ERROR_OK;
}
error_t bypass_process(void* dat, p_buffer_t in, p_buffer_t out)
{
    *out = *in;
    return ERROR_OK;
}

_Q16 hard_clipping_gain_coefs[] = {
#include "./precomputes/hc_gain_coefs.dat"
};
_Q16 hard_clipping_filter_coefs[] = {
#include "./precomputes/hc_filter_coefs.dat"
};
error_t hard_clipping_init(hard_clipping *hc, p_buffer_t buf)
{
    hc->gain = 0;

    fsaver_result fsr = no_error;
    hc->filter_coefs = iir_get__Q16(hard_clipping_filter_coefs, 0, &fsr);
    if(fsr)
        return ALGORITHM_INITIALIZATION_ERROR;

    //Clean up filters buffer
    unsigned int i = 0;
    for(i = 0; i < 4; i++)
        hc->filter_buf[i] = 0;

    return ERROR_OK;
}
error_t hard_clipping_set_params(void* dat, unsigned int num, unsigned int val)
{
    hard_clipping* hc = dat;

    if(val <= MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    fsaver_result fsr = no_error;
    hc->gain = *plane_data_get__Q16(hard_clipping_gain_coefs, val, &fsr);
    if(fsr)
        return PARAMS_SET_ERROR;

    return ERROR_OK;
}

error_t hard_clipping_process(void *dat, p_buffer_t in, p_buffer_t out)
{
    hard_clipping* hc = dat;

    _Q16 sample = Q15toQ16(*in);

    //Applay gain
    sample = Q16mpy(sample, hc->gain);

    //Clip
    if(sample >= _Q16ftoi(0.7))
        sample = _Q16ftoi(0.7);
    if(sample < _Q16ftoi(-0.7))
        sample =  _Q16ftoi(-0.7);

    //Post-filter
    sample = DF2SOStructure(sample,
            (_Q16*)&hc->filter_coefs[0], (_Q16*)&hc->filter_coefs[3],
            hc->filter_buf, &hc->filter_buf[2]);

    return Q16toQ15(sample);
}

_Q16 soft_clipping_gain_coefs[] = {
#include "./precomputes/sc_gain_coefs.dat"
};
_Q16 soft_clipping_filter_coefs[] = {
#include "./precomputes/sc_filter_coefs.dat"
};
error_t soft_clipping_init(soft_clipping *sc, p_buffer_t buf)
{
    sc->gain = 0;

    fsaver_result fsr = no_error;
    sc->filter_coefs = iir_get__Q16(soft_clipping_filter_coefs, 0, &fsr);
    if(fsr)
        return ALGORITHM_INITIALIZATION_ERROR;

    //Clean up filters buffer
    unsigned int i = 0;
    for(i = 0; i < 4; i++)
        sc->filter_buf[i] = 0;

    return ERROR_OK;
}
error_t soft_clipping_set_params(void* dat, unsigned int num, unsigned int val)
{
    soft_clipping* sc = dat;

    if(val <= MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    fsaver_result fsr = no_error;
    sc->gain = *plane_data_get__Q16(soft_clipping_gain_coefs, val, &fsr);
    if(fsr)
        return PARAMS_SET_ERROR;

    return ERROR_OK;
}
error_t soft_clipping_process(void *dat, p_buffer_t in, p_buffer_t out)
{
    //(_*9:_+1:log10(_)),(_*-1:_*9:_+1:log10(_)
 
    soft_clipping* sc = dat;

    _Q16 sample = Q15toQ16(*in);

    //Applay gain
    sample = Q16mpy(sample, sc->gain);

    //Try logarithmic clipping
    if(sample >= 0)
    {
        sample = Q16mpy(sample,_Q16ftoi(9.0));
        sample += 1.0;
        sample = _Q16log10(sample);

    }
    else
    {
        sample = Q16mpy(sample,_Q16ftoi(-1.0));
        sample = Q16mpy(sample,_Q16ftoi(9.0));
        sample += 1.0;
        sample = _Q16log10(sample);
        sample = Q16mpy(sample,_Q16ftoi(-1.0));
    }

    //Then - hard clipping
    if(sample >= _Q16ftoi(0.99))
        sample = _Q16ftoi(0.99);
    if(sample < _Q16ftoi(-0.99))
        sample =  _Q16ftoi(-0.99);

    //Post-filter
    sample = DF2SOStructure(sample,
            (_Q16*)&sc->filter_coefs[0], (_Q16*)&sc->filter_coefs[3],
            sc->filter_buf, &sc->filter_buf[2]);

    return Q16toQ15(sample);
}

error_t compression_init(compression *c, p_buffer_t buf)
{
    c->ctr = 0;
    c->env_buf = 0;

    return ERROR_OK;
}
error_t compression_set_params(void* dat, unsigned int num, unsigned int val)
{
    compression* c = dat;

    if(val <= MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    c->ctr = val;

    return ERROR_OK;
}
error_t compression_process(void *dat, p_buffer_t in, p_buffer_t out)
{
    /*
    _Q16 in = Q15toQ16(sample);
    _Q16 e = 0;
    _Q16 out = 0;

    e = compression_buf + Q16mpy((Q16mpy(in, in) - compression_buf),COMPRESSION_TAV);
    compression_buf = e;

    out = Q16mpy((Q16ftoi(1.0) - Q16mpy(compression_dat[parameter_val + 1],e)),in);

    if(in > _Q16ftoi(0.999))
        in = _Q16ftoi(0.999);
    if(in < _Q16ftoi(-0.999))
        in = _Q16ftoi(-0.999);

    return Q16toQ15(out);
     */

    *out = *in;
    return ERROR_OK;
}

_Q16 lp_filter_buf[4];
_Q15 lp_filter(_Q15 sample, unsigned int parameter_val)
{
	//Integrate MATLAB data saver
    _Q15 out = 0;

    _Q16 b[3];
    _Q16 a[3];

    //Coefs order:a0,a1,a2,b0,b1,b2
    a[0] = lp_filter_dat[parameter_val];
    a[1] = lp_filter_dat[parameter_val + 1*LP_FILTER_COEFS_SETS_NUM];
    a[2] = lp_filter_dat[parameter_val + 2*LP_FILTER_COEFS_SETS_NUM];

    b[0] = lp_filter_dat[parameter_val + 3*LP_FILTER_COEFS_SETS_NUM];
    b[1] = lp_filter_dat[parameter_val + 4*LP_FILTER_COEFS_SETS_NUM];
    b[2] = lp_filter_dat[parameter_val + 5*LP_FILTER_COEFS_SETS_NUM];

    out = Q16toQ15(DF2SOStructure(Q15toQ16(sample), b, a, lp_filter_buf, &lp_filter_buf[2]));

    return out;
}

_Q16 bp_filter_buf[4];
_Q15 bp_filter(_Q15 sample, unsigned int parameter_val)
{
    _Q15 out = 0;

    _Q16 b[3];
    _Q16 a[3];

    a[0] = bp_filter_dat[parameter_val];
    a[1] = bp_filter_dat[parameter_val + 1*BP_FILTER_COEFS_SETS_NUM];
    a[2] = bp_filter_dat[parameter_val + 2*BP_FILTER_COEFS_SETS_NUM];

    b[0] = bp_filter_dat[parameter_val + 3*BP_FILTER_COEFS_SETS_NUM];
    b[1] = bp_filter_dat[parameter_val + 4*BP_FILTER_COEFS_SETS_NUM];
    b[2] = bp_filter_dat[parameter_val + 5*BP_FILTER_COEFS_SETS_NUM];

    out = Q16toQ15(DF2SOStructure(Q16mpy(Q15toQ16(sample), BP_FILTER_INPUT_COEF), b, a, bp_filter_buf, &bp_filter_buf[2]));

    return out;
}

_Q16 hp_filter_buf[4];
_Q15 hp_filter(_Q15 sample, unsigned int parameter_val)
{

    _Q15 out = 0;

    _Q16 b[3];
    _Q16 a[3];

    a[0] = hp_filter_dat[parameter_val];
    a[1] = hp_filter_dat[parameter_val + 1*HP_FILTER_COEFS_SETS_NUM];
    a[2] = hp_filter_dat[parameter_val + 2*HP_FILTER_COEFS_SETS_NUM];

    b[0] = hp_filter_dat[parameter_val + 3*HP_FILTER_COEFS_SETS_NUM];
    b[1] = hp_filter_dat[parameter_val + 4*HP_FILTER_COEFS_SETS_NUM];
    b[2] = hp_filter_dat[parameter_val + 5*HP_FILTER_COEFS_SETS_NUM];

    out = Q16toQ15(DF2SOStructure(Q15toQ16(sample), b, a, hp_filter_buf, &hp_filter_buf[2]));

    return out;
}
/*
unsigned int chorus_counters[6] = {0,0,1500,1500,0,0};
_Q15 chorus_fb_point[2];
_Q15 chorus(_Q15 sample, unsigned int parameter_val)
{

    sample = Q15mpy(sample,CHORUS_INPUT_COEFS(0));
    sample = Q15mpy(sample,CHORUS_INPUT_COEFS(1))
        + Q15mpy(CHORUS_FEEDFORWARD_COEFS(0),li_delay_line(sample + chorus_fb_point[0] , &mod_effects_buf[0], &chorus_counters[0], CHORUS_BUF_LEN, chorus_dat[chorus_counters[3]], chorus_dat[chorus_counters[3]+CHORUS_WAVE_TABLE_LEN]));
        + Q15mpy(CHORUS_FEEDFORWARD_COEFS(1),li_delay_line(sample + chorus_fb_point[1], &mod_effects_buf[CHORUS_BUF_LEN], &chorus_counters[1], CHORUS_BUF_LEN, chorus_dat[chorus_counters[5]], chorus_dat[chorus_counters[5]]+CHORUS_WAVE_TABLE_LEN));

    chorus_fb_point[0] = Q15mpy(CHORUS_FEEDBACK_COEFS(0), delay_line_tap(CHORUS_TAP_LENS(0), &mod_effects_buf[0], chorus_counters[0], CHORUS_BUF_LEN));
    chorus_fb_point[1] = Q15mpy(CHORUS_FEEDBACK_COEFS(1), delay_line_tap(CHORUS_TAP_LENS(1), &mod_effects_buf[CHORUS_BUF_LEN], chorus_counters[1], CHORUS_BUF_LEN));

    //Chorus counters update
    chorus_counters[2]++;
    if (chorus_counters[2] >= (13*(1+MAX_PARAMETER_VAL-parameter_val)))
    {
        chorus_counters[3]++;
        if (chorus_counters[3] >= CHORUS_WAVE_TABLE_LEN)
            chorus_counters[3]=0;
        chorus_counters[2]=0;
    }

    chorus_counters[4]++;
    if (chorus_counters[4] >= (7*(1+MAX_PARAMETER_VAL-parameter_val)))
    {
        chorus_counters[5]++;
        if (chorus_counters[5] >= CHORUS_WAVE_TABLE_LEN)
            chorus_counters[5] = 0;
        chorus_counters[4]=0;
    }

    return sample;
}

unsigned int flange_counter;
unsigned int flange_wt_counter0;
unsigned int flange_wt_counter1;
_Q15 flange_fb_point;
_Q15 flange(_Q15 sample, unsigned int parameter_val)
{

    sample = Q15mpy(sample + flange_fb_point, FLANGE_INPUT_COEF) +
                Q15mpy(FLANGE_FEEDFORWARD_COEF, li_delay_line(sample, &mod_effects_buf[0], &flange_counter,FLANGE_BUF_LEN, flange_dat[flange_wt_counter1], flange_dat[flange_wt_counter1 + FLANGE_WAVE_TABLE_LEN]));
    flange_fb_point = Q15mpy(FLANGE_FEEDBACK_COEF, delay_line_tap(FLANGE_TAP_LEN, &mod_effects_buf[0], flange_counter, FLANGE_BUF_LEN));

    flange_wt_counter0++;
    if (flange_wt_counter0 >= (1+(MAX_PARAMETER_VAL-parameter_val)))
    {
        flange_wt_counter1++;
        if (flange_wt_counter1 >= FLANGE_WAVE_TABLE_LEN)
            flange_wt_counter1 = 0;
        flange_wt_counter0 = 0;
    }

    return sample;
}

_Q15 tremolo_counter;
_Q15 tremolo(_Q15 sample, unsigned int parameter_val)
{
    sample = Q15mpy(sample,Q15mpy(_Q15ftoi(0.5), _Q15sinPI(tremolo_counter)));

    tremolo_counter+=2*(parameter_val+1);
    if (tremolo_counter >= 32767)
        tremolo_counter = -32767;

    return sample;
}





unsigned int delay_counter;
_Q15 delay(_Q15 sample, unsigned int parameter_val)
{
    sample = Q15mpy(DELAY_FF_COEF,sample) + Q15mpy(DELAY_TAP_COEF, delay_line(sample, delay_effects_buf, &delay_counter, (DELAY_BUF_LEN/10)*parameter_val + 1));

    return sample;
}

_Q15 echo_fb_point;
unsigned int echo_counter;
_Q15 echo(_Q15 sample, unsigned int parameter_val)
{
    _Q15 p0 = 0;
    _Q15 p1 = 0;
    
    p0 = sample + Q15mpy(ECHO_FB_COEF, Q15mpy(int_to_Q15[parameter_val],echo_fb_point));
    p1 = Q15mpy(int_to_Q15[parameter_val],Q15mpy(ECHO_TAP_COEF, delay_line(p0, delay_effects_buf, &echo_counter, ECHO_BUF_LEN)));

    sample = Q15mpy(ECHO_FF_COEF, p0) + p1;

    echo_fb_point = p1;

    return sample;
}void *dat,

_Q15 reverb_fb_p;
unsigned int reverb_counter[3];
_Q15 reverb(_Q15 sample, unsigned int parameter_val)                                                                  
{
	//Implement something like Schoeders 1
    sample = Q15mpy(REVERB_FF_COEF, sample) + Q15mpy(Q15mpy(REVERB_FB_COEF,int_to_Q15[parameter_val]),reverb_fb_p);
    sample += Q15mpy(REVERB_TAP_COEFS(3), delay_line(sample, delay_effects_buf, &reverb_counter[0], REVERB_BUF_LEN));
    sample += Q15mpy(REVERB_TAP_COEFS(0), delay_line_tap(REVERB_TAP_LENS(0), delay_effects_buf, reverb_counter[0], REVERB_BUF_LEN));
    sample += Q15mpy(REVERB_TAP_COEFS(1), delay_line_tap(REVERB_TAP_LENS(1), delay_effects_buf, reverb_counter[0], REVERB_BUF_LEN));
    sample += Q15mpy(REVERB_TAP_COEFS(2), delay_line_tap(REVERB_TAP_LENS(2), delay_effects_buf, reverb_counter[0], REVERB_BUF_LEN));
    reverb_fb_p = sample;
    sample = all_pass(sample, REVERB_APF_COEFS(0), &delay_effects_buf[REVERB_BUF_LEN], &reverb_counter[1], REVERB_APF_LENS(0));
    sample = all_pass(sample, REVERB_APF_COEFS(1), &delay_effects_buf[REVERB_BUF_LEN + REVERB_APF_LENS(0)], &reverb_counter[2], REVERB_APF_LENS(1));
    
    return sample;
}

*/



