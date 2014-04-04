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
#include <stdlib.h>

#include <libq.h>

#include "MatlabDefs.h"
//Here will be included all matlab related data

#include "FoundAlgorithms.h"

#include "Effects.h"

error_t bypass_init(bypass *bp, pBuffer_t buf, initialization_t identificationType)
{
	return ERROR_OK;
}
error_t bypass_set_params(void* str, paramNum_t paramNumber, unsigned int val)
{
	return ERROR_OK;
}
error_t bypass_process(void* str, pBuffer_t in, pBuffer_t out)
{
	*out = *in;
}

_Q16 hard_clipping_prefilter_buf[4];
_Q15 hard_clipping(_Q15 sample, unsigned int parameter_val)
{   
    _Q16 in = Q15toQ16(sample);

    in = Q16mpy(in, hard_clipping_dat[parameter_val + 6]);

    if(in >= _Q16ftoi(0.3))
        in = _Q16ftoi(0.3);
    if(in < _Q16ftoi(-0.3))
        in =  _Q16ftoi(-0.3);

    //post-filter
    in = DF2SOStructure(in, (_Q16*)&hard_clipping_dat[0], (_Q16*)&hard_clipping_dat[3], hard_clipping_prefilter_buf, &hard_clipping_prefilter_buf[2]);

    return Q16toQ15(in);
}

_Q16 soft_clipping_prefilter_buf[4];
_Q15 soft_clipping(_Q15 sample, unsigned int parameter_val)
{
	//Try to swap to hiperbolic sinus if exists
    _Q16 in = Q15toQ16(sample);
    _Q16 out = 0;

    in = Q16mpy(in, soft_clipping_dat[parameter_val + 6]);
  
    if((_Q16ftoi(-0.35) <= in) && (in <= _Q16ftoi(0.35)))
                in = _Q16sin(Q16mpy(in,_Q16ftoi(1.3)));

    if(in > _Q16ftoi(0.35))
        in = _Q16ftoi(0.35);
    if(in < _Q16ftoi(-0.35))
        in = _Q16ftoi(-0.35);

    //post-filter
    out = DF2SOStructure(in, (_Q16*)&soft_clipping_dat[0], (_Q16*)&soft_clipping_dat[3], soft_clipping_prefilter_buf, &soft_clipping_prefilter_buf[2]);

    return Q16toQ15(out);
}

_Q16 compression_buf;
_Q15 compression(_Q15 sample, unsigned int parameter_val)
{
	//ПЕРЕДЕЛАТЬ НАХУЙ, add envelope follower
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
}

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

void signal_fork(_Q15 sample, _Q15* out_sample_L, _Q15* out_sample_R)
{
     *out_sample_L = sample;
     *out_sample_R = sample;
}




