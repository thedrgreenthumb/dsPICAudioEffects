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
#include <p33FJ128GP802.h> //!!!Remove it, only for debug purposes

#include <libq.h>

//Matlab
#include "matlab/@fsaver/fsaver.h"
//Declare fsaver functions for Q formats
fsaver_plane_data_declare(_Q15)
fsaver_plane_data_declare(_Q16)
fsaver_iir_declare(_Q16)

#include "foundation.h"
#include "runner/runner.h"
#include "effects.h"

_Q15 dc_blocker_buf[2];
_Q15 w_dc_blocker(_Q15 sample)
{
    return dc_blocker(sample, _Q15ftoi(0.005), dc_blocker_buf);
}

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

    if(val > MAX_PARAMETER_VAL)
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
    if(sample >= _Q16ftoi(0.15))
        sample = _Q16ftoi(0.15);
    if(sample < _Q16ftoi(-0.15))
        sample =  _Q16ftoi(-0.15);

    //Post-filter
    sample = DF2SOStructure(sample,
            (_Q16*)&hc->filter_coefs[0], (_Q16*)&hc->filter_coefs[3],
            hc->filter_buf, &hc->filter_buf[2]);

    *out = Q16toQ15(sample);

    return ERROR_OK;
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

    if(val > MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    fsaver_result fsr = no_error;
    sc->gain = *plane_data_get__Q16(soft_clipping_gain_coefs, val, &fsr);
    if(fsr)
        return PARAMS_SET_ERROR;

    return ERROR_OK;
}
error_t soft_clipping_process(void *dat, p_buffer_t in, p_buffer_t out)
{
    soft_clipping* sc = dat;

    _Q16 sample = Q15toQ16(*in);

    //Applay gain
    sample = Q16mpy(sample, sc->gain);

    //Try logarithmic clipping
    if(sample >= 0)
    {
        sample = Q16mpy(sample,_Q16ftoi(9.0));
        sample += _Q16ftoi(1.0);
        sample = _Q16log10(sample);

    }
    else
    {
        sample = Q16mpy(sample,_Q16ftoi(-1.0));
        sample = Q16mpy(sample,_Q16ftoi(9.0));
        sample += _Q16ftoi(1.0);
        sample = _Q16log10(sample);
        sample = Q16mpy(sample,_Q16ftoi(-1.0));
    }

    //Then - hard clipping
    if(sample >= _Q16ftoi(0.99))
        sample = _Q16ftoi(0.99);
    if(sample < _Q16ftoi(-0.99))
        sample =  _Q16ftoi(-0.99);

    //Decrease amplitude to match with bypass mode
    sample = Q16mpy(sample,_Q16ftoi(0.15));
    
    //Post-filter
    sample = DF2SOStructure(sample,
            (_Q16*)&sc->filter_coefs[0], (_Q16*)&sc->filter_coefs[3],
            sc->filter_buf, &sc->filter_buf[2]);

    *out = Q16toQ15(sample);

    return ERROR_OK;
}

_Q16 compressor_gain_coefs[] = {
#include "./precomputes/comp_coefs.dat"
};

error_t compression_init(compression *c, p_buffer_t buf)
{
    c->ctr = 0;

    return ERROR_OK;
}
error_t compression_set_params(void* dat, unsigned int num, unsigned int val)
{
    compression* c = dat;

    if(val > MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    fsaver_result fsr = no_error;
    c->ctr = *plane_data_get__Q16(compressor_gain_coefs, val, &fsr);
    if(fsr)
        return PARAMS_SET_ERROR;

    return ERROR_OK;
}
error_t compression_process(void *dat, p_buffer_t in, p_buffer_t out)
{
    compression* c = dat;
    _Q16 sample = Q15toQ16(*in);

    //Applay gain
    sample = Q16mpy(sample, c->ctr);

    //Try logarithmic compression
    if(sample >= 0)
    {
        sample = Q16mpy(sample,_Q16ftoi(9.0));
        sample += _Q16ftoi(1.0);
        sample = _Q16log10(sample);
    }
    else
    {
        sample = Q16mpy(sample,_Q16ftoi(-1.0));
        sample = Q16mpy(sample,_Q16ftoi(9.0));
        sample += _Q16ftoi(1.0);
        sample = _Q16log10(sample);
        sample = Q16mpy(sample,_Q16ftoi(-1.0));
    }

    //Then - hard clipping
    if(sample >= _Q16ftoi(0.99))
        sample = _Q16ftoi(0.99);
    if(sample < _Q16ftoi(-0.99))
        sample =  _Q16ftoi(-0.99);

    //Decrease amplitude to match with bypass mode
    sample = Q16mpy(sample,_Q16ftoi(0.15));

    *out = Q16toQ15(sample);

    return ERROR_OK;
}

_Q16 low_pass_filter_coefs[] = {
#include "./precomputes/lp_filter_coefs.dat"
};
error_t lp_filter_init(lp_filter *lp, p_buffer_t buf)
{
    lp->freq = 0;

    unsigned int i = 0;
    for(i = 0; i < 4; i++)
        lp->filter_buf[i] = 0;

    return ERROR_OK;
}
error_t lp_filter_set_params(void* dat, unsigned int num, unsigned int val)
{
    lp_filter* lp = dat;

    if(val > MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    lp->freq = val;

    return ERROR_OK;
}
error_t lp_filter_process(void *dat, p_buffer_t in, p_buffer_t out)
{
    lp_filter* lp = dat;

    _Q16 sample = Q15toQ16(*in);

    fsaver_result fsr = no_error;
    const _Q16* coefs = iir_get__Q16(low_pass_filter_coefs, lp->freq, &fsr);
    if(fsr)
        return PARAMS_SET_ERROR;

    sample = DF2SOStructure(sample, coefs, &coefs[3],
            lp->filter_buf, &lp->filter_buf[2]);

    *out = Q16toQ15(sample);

    return ERROR_OK;
}

_Q16 band_pass_filter_coefs[] = {
#include "./precomputes/bp_filter_coefs.dat"
};
_Q16 band_pass_gain_coefs[] = {
#include "./precomputes/bp_filter_gain_coefs.dat"
};
error_t bp_filter_init(bp_filter *bp, p_buffer_t buf)
{
    bp->freq = 0;

    fsaver_result fsr = no_error;
    bp->gain_coef = *plane_data_get__Q16(band_pass_gain_coefs, 0, &fsr);
    if(fsr)
        return ALGORITHM_INITIALIZATION_ERROR;

    unsigned int i = 0;
    for(i = 0; i < 4; i++)
        bp->filter_buf[i] = 0;

    return ERROR_OK;
}
error_t bp_filter_set_params(void* dat, unsigned int num, unsigned int val)
{
    bp_filter* bp = dat;

    if(val > MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    bp->freq = val;

    return ERROR_OK;
}
error_t bp_filter_process(void *dat, p_buffer_t in, p_buffer_t out)
{
    bp_filter* bp = dat;

    _Q16 sample = Q15toQ16(*in);

    fsaver_result fsr = no_error;
    const _Q16* coefs = iir_get__Q16(band_pass_filter_coefs, bp->freq, &fsr);
    if(fsr)
        return PARAMS_SET_ERROR;

    sample = DF2SOStructure(sample, coefs, &coefs[3],
            bp->filter_buf, &bp->filter_buf[2]);

    sample = Q16mpy(sample, bp->gain_coef);

    *out = Q16toQ15(sample);

    return ERROR_OK;
}

_Q16 high_pass_filter_coefs[] = {
#include "./precomputes/hp_filter_coefs.dat"
};
error_t hp_filter_init(hp_filter *hp, p_buffer_t buf)
{
    hp->freq = 0;

    unsigned int i = 0;
    for(i = 0; i < 4; i++)
        hp->filter_buf[i] = 0;

    return ERROR_OK;
}
error_t hp_filter_set_params(void* dat, unsigned int num, unsigned int val)
{
    hp_filter* hp = dat;

    if(val > MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    hp->freq = val;

    return ERROR_OK;
}
error_t hp_filter_process(void *dat, p_buffer_t in, p_buffer_t out)
{
    hp_filter* hp = dat;

    _Q16 sample = Q15toQ16(*in);

    fsaver_result fsr = no_error;
    const _Q16* coefs = iir_get__Q16(high_pass_filter_coefs, hp->freq, &fsr);
    if(fsr)
        return PARAMS_SET_ERROR;

    sample = DF2SOStructure(sample, coefs, &coefs[3],
            hp->filter_buf, &hp->filter_buf[2]);

    *out = Q16toQ15(sample);

    return ERROR_OK;
}

_Q15 chorus_coefs[] = {
#include "./precomputes/chorus_coefs.dat"
};
_Q15 chorus_wave_table[] = {
#include "./precomputes/chorus_wave_table.dat"
};
error_t chorus_init(chorus *ch, p_buffer_t buf)
{
    /*
    ch->buf = buf;
    ch->buf_len = buf[0];
    
    fsaver_result fsr = no_error;
    ch->w_int_part = (unsigned int*)plane_data_get__Q15(chorus_wave_table, 0, &fsr);
    ch->w_frac_part = plane_data_get__Q15(chorus_wave_table, 1, &fsr);
    if(fsr)
        return PARAMS_SET_ERROR;
    
    unsigned int i = 0;
    for(i = 0; i < 6; i++)
        ch->counters[i] = 0;
    
    ch->fb_points[0] = 0;
    ch->fb_points[1] = 0;

    ch->in_coefs = plane_data_get__Q15(chorus_coefs, 0, &fsr);
    ch->fb_coefs = plane_data_get__Q15(chorus_coefs, 1, &fsr);
    ch->ff_coefs = plane_data_get__Q15(chorus_coefs, 2, &fsr);
    if(fsr)
        return PARAMS_SET_ERROR;
    */
    return ERROR_OK;
}
error_t chorus_set_params(void* dat, unsigned int num, unsigned int val)
{
    chorus* ch = dat;

    if(val <= MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    ch->ctr = val;

    return ERROR_OK;
}
error_t chorus_process(void* dat, p_buffer_t in, p_buffer_t out)
{
    //chorus* ch = dat;
    _Q15 sample = *in;

    /*
    sample = Q15mpy(sample, ch->in_coefs[0]);
    sample = Q15mpy(sample, ch->in_coefs[1]);
    sample = _Q15add(sample, )
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
    */
   *out = sample;



    return ERROR_OK;
}

error_t flange_init(flange *bp, p_buffer_t buf)
{
    return ERROR_OK;
}
error_t flange_set_params(void* dat, unsigned int num, unsigned int val)
{
    return ERROR_OK;
}
error_t flange_process(void* dat, p_buffer_t in, p_buffer_t out)
{
    *out = *in;
    return ERROR_OK;
}
/*

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
*/


error_t tremolo_init(tremolo *tr, p_buffer_t buf)
{
    tr->tremolo_counter = 0;
    tr->freq = 0;
    return ERROR_OK;
}
error_t tremolo_set_params(void* dat, unsigned int num, unsigned int val)
{
    tremolo* hp = dat;

    if(val <= MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    hp->freq = val;

    return ERROR_OK;
}
error_t tremolo_process(void* dat, p_buffer_t in, p_buffer_t out)
{
    tremolo *tr = dat;

    _Q15 sample = Q15mpy(*in, Q15mpy(_Q15ftoi(0.5), _Q15sinPI(tr->tremolo_counter)));

    tr->tremolo_counter += 2*(tr->freq + 1);
    if (tr->tremolo_counter >= 32767)
        tr->tremolo_counter = -32767;

    *out = sample;

    return ERROR_OK;
}

error_t delay_init(delay *dl, p_buffer_t buf)
{
    dl->time = 0;
    dl->counter = 0;

    dl->ff_coef = Q15ftoi(0.7);
    dl->tap_coef = Q15ftoi(0.3);

    dl->buf = buf;
    dl->buf_len = buf[0];

    return ERROR_OK;
}
error_t delay_set_params(void* dat, unsigned int num, unsigned int val)
{
    delay* dl = dat;

    if(val <= MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    dl->time = val;

    return ERROR_OK;
}
error_t delay_process(void* dat, p_buffer_t in, p_buffer_t out)
{
    delay *dl = dat;

    _Q15 sample = *in;

    *out = Q15mpy(dl->ff_coef,*in)
            + Q15mpy(dl->tap_coef, //Add one to parameter to avoid delay line with zero length
            delay_line(sample, dl->buf, &dl->counter, (dl->buf_len/10)*dl->time + 1));

    *out = sample;

    return ERROR_OK;
}

error_t echo_init(echo *ech, p_buffer_t buf)
{
    ech->time = 0;
    ech->counter = 0;

    ech->fb_point = 0;

    ech->buf = buf;
    ech->buf_len = buf[0];

    ech->ff_coef = Q15ftoi(0.3);
    ech->fb_coef = Q15ftoi(0.3);
    ech->tap_coef = Q15ftoi(0.3);

    return ERROR_OK;
}
error_t echo_set_params(void* dat, unsigned int num, unsigned int val)
{
    echo* ech = dat;

    if(val <= MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    ech->time = val;

    return ERROR_OK;
}
error_t echo_process(void* dat, p_buffer_t in, p_buffer_t out)
{
    echo* ech = dat;

    _Q15 sample = *in;
    _Q15 p0 = 0;
    _Q15 p1 = 0;
    
    p0 = _Q15add(sample, Q15mpy(ech->fb_coef, Q15mpy(itoQ15div10(ech->time), ech->fb_point)));
    p1 = Q15mpy(itoQ15div10(ech->time),Q15mpy(ech->tap_coef, delay_line(p0, ech->buf, &ech->counter, ech->buf_len)));

    sample = _Q15add(Q15mpy(ech->ff_coef, p0), p1);

    ech->fb_point = p1;

    *out = sample;

    return ERROR_OK;
}

error_t reverb_init(reverb *rev, p_buffer_t buf)
{
    rev->rt = 0;

    rev->buf = buf;

    rev->fb_point = 0;

    rev->early_counter = 0;
    rev->ap_counters[0] = 0;
    rev->ap_counters[1] = 0;

    rev->ff_coef = Q15ftoi(0.5);
    rev->fb_coef = Q15ftoi(0.25);

    rev->tap_coefs[0] = Q15ftoi(0.2);
    rev->tap_coefs[1] = Q15ftoi(-0.25);
    rev->tap_coefs[2] = Q15ftoi(0.15);
    rev->tap_coefs[3] = Q15ftoi(0.11);

    rev->tap_sizes[0] = 1521;
    rev->tap_sizes[1] = 2963;
    rev->tap_sizes[2] = 3859;

    rev->ap_coefs[0] = Q15ftoi(0.23);
    rev->ap_coefs[1] = Q15ftoi(0.17);

    rev->ap_sizes[0] = 357;
    rev->ap_sizes[1] = 129;

    //Provide early buf size,
    //full buf szie passed thru first element of array
    rev->buf_len = rev->buf[0] - rev->ap_sizes[0] - rev->ap_sizes[1];

    return ERROR_OK;
}
error_t reverb_set_params(void* dat, unsigned int num, unsigned int val)
{
    reverb* rev = dat;

    if(val <= MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    rev->rt = val;

    return ERROR_OK;
}
error_t reverb_process(void* dat, p_buffer_t in, p_buffer_t out)
{
    reverb* rev = dat;

    _Q15 sample = *in;

    sample = _Q15add(Q15mpy(rev->ff_coef, sample),
            Q15mpy(Q15mpy(rev->fb_coef,itoQ15div10(rev->rt)),rev->fb_point));
    //Del line
    sample = _Q15add(sample, Q15mpy(rev->tap_coefs[0],
            delay_line(sample, rev->buf, &rev->early_counter, rev->buf_len)));
    //Taps
    unsigned int i = 0;
    for(i = 0; i < 3; i++)
        sample = _Q15add(sample, Q15mpy(rev->tap_coefs[i+1],
                delay_line_tap(rev->tap_sizes[i], rev->buf, rev->early_counter, rev->buf_len)));
  
    rev->fb_point = sample;

    //Allpass filters
    sample = all_pass(sample, rev->ap_coefs[0], &rev->buf[rev->buf_len], &rev->ap_counters[0], rev->ap_sizes[0]);
    sample = all_pass(sample, rev->ap_coefs[1], &rev->buf[rev->buf_len + rev->ap_sizes[0]], &rev->ap_counters[1], rev->ap_sizes[1]);

    *out = sample;

    return ERROR_OK;
}





