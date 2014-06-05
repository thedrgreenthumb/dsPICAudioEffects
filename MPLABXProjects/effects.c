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
* Project version: 0.01
**********************************************************************/

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

const _Q16 hard_clipping_gain_coefs[] = {
#include "./precomputes/hc_gain_coefs.dat"
};
const _Q16 hard_clipping_filter_coefs[] = {
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

    //Apply gain
    sample = Q16mpy(sample, hc->gain);

    //Clip
    if(sample >= _Q16ftoi(0.15))
        sample = _Q16ftoi(0.15);
    if(sample < _Q16ftoi(-0.15))
        sample =  _Q16ftoi(-0.15);

    //Post-filter
    sample = DF2SOStructure(sample,
            &hc->filter_coefs[0], &hc->filter_coefs[3],
            hc->filter_buf, &hc->filter_buf[2]);

    *out = Q16toQ15(sample);

    return ERROR_OK;
}

const _Q16 soft_clipping_gain_coefs[] = {
#include "./precomputes/sc_gain_coefs.dat"
};
const _Q16 soft_clipping_filter_coefs[] = {
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

const _Q16 compressor_gain_coefs[] = {
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

    //Apply gain
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

const _Q16 low_pass_filter_coefs[] = {
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

const _Q16 band_pass_filter_coefs[] = {
#include "./precomputes/bp_filter_coefs.dat"
};
const _Q16 band_pass_gain_coefs[] = {
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

    //sample = Q16mpy(sample, bp->gain_coef);

    *out = Q16toQ15(sample);

    return ERROR_OK;
}

const _Q16 high_pass_filter_coefs[] = {
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

const int chorus_wt_int[] = {
#include "./precomputes/chorus_wt_int.dat"
};
const _Q15 chorus_wt_fract[] = {
#include "./precomputes/chorus_wt_frac.dat"
};
error_t chorus_init(chorus *ch, p_buffer_t buf)
{
    ch->op_buf = buf;
    ch->op_buf_sz = buf[0];

    ch->dl1_buf = ch->op_buf;
    ch->dl1_buf_sz = ch->op_buf_sz/2;
    ch->dl2_buf = &ch->op_buf[ch->dl1_buf_sz];
    ch->dl2_buf_sz = ch->op_buf_sz/2;

    ch->wt_size = 3000; //See matlab file
    fsaver_result fsr = no_error;
    ch->w_int_part = plane_data_get_int(chorus_wt_int, 0, &fsr);
    if(fsr)
        return PARAMS_SET_ERROR;
    
    ch->w_frac_part = plane_data_get__Q15(chorus_wt_fract, 0, &fsr);
    if(fsr)
        return PARAMS_SET_ERROR;
    
    unsigned int i = 0;
    for(i = 0; i < 6; i++)
        ch->counters[i] = 0;
    
    ch->in_coefs[0] = Q15ftoi(0.3); ch->in_coefs[1] = Q15ftoi(0.3);
    ch->fb_coefs[0] = Q15ftoi(0.5); ch->fb_coefs[1] = Q15ftoi(0.5);
    ch->ff_coefs[0] = Q15ftoi(0.3); ch->ff_coefs[0] = Q15ftoi(0.3);

    ch->tap_szs[0] = ch->dl1_buf_sz/2;
    ch->tap_szs[1] = ch->dl2_buf_sz/3;

    ch->fb_points[0] = 0; ch->fb_points[1] = 0;

    return ERROR_OK;
}
error_t chorus_set_params(void* dat, unsigned int num, unsigned int val)
{
    chorus* ch = dat;

    if(val > MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    ch->ctr = val;

    return ERROR_OK;
}
error_t chorus_process(void* dat, p_buffer_t in, p_buffer_t out)
{
    chorus* ch = dat;
    _Q15 sample1 = *in;
    _Q15 sample2 = *in;

    sample1 = _Q15add(Q15mpy(_Q15add(sample1, ch->fb_points[0]), ch->in_coefs[0]),
                Q15mpy(ch->ff_coefs[0], li_delay_line(sample1, ch->dl1_buf, 
            &ch->counters[0],ch->dl1_buf_sz, ch->w_int_part[ch->counters[3]], 
            ch->w_frac_part[ch->counters[3]])));
   
    ch->fb_points[0] = Q15mpy(ch->fb_coefs[0], 
            delay_line_tap(ch->tap_szs[0], ch->dl1_buf, ch->counters[0], ch->dl1_buf_sz));
    
    sample2 = _Q15add(Q15mpy(_Q15add(sample2, ch->fb_points[1]), ch->in_coefs[1]),
                Q15mpy(ch->ff_coefs[1], li_delay_line(sample2, ch->dl2_buf, 
            &ch->counters[1],ch->dl2_buf_sz, ch->w_int_part[ch->counters[5]], 
            ch->w_frac_part[ch->counters[5]])));
   
    ch->fb_points[1] = Q15mpy(ch->fb_coefs[1], 
            delay_line_tap(ch->tap_szs[1], ch->dl2_buf, ch->counters[1], ch->dl2_buf_sz));

    _Q15 sample = _Q15add(Q15mpy(Q15ftoi(0.5),sample1), Q15mpy(Q15ftoi(0.5),sample2));

    //Chorus counters update
    ch->counters[2]++;
    if (ch->counters[2] >= (13*(1 + MAX_PARAMETER_VAL - ch->ctr)))
    {
        ch->counters[3]++;
        if (ch->counters[3] >= ch->wt_size)
            ch->counters[3]=0;
        ch->counters[2]=0;
    }

    ch->counters[4]++;
    if (ch->counters[4] >= (7*(1 + MAX_PARAMETER_VAL - ch->ctr)))
    {
        ch->counters[5]++;
        if (ch->counters[5] >= ch->wt_size)
            ch->counters[5] = 0;
        ch->counters[4]=0;
    }
   
   *out = sample;

    return ERROR_OK;
}

const int flange_wt_int[] = {
#include "./precomputes/flange_wt_int.dat"
};
const _Q15 flange_wt_fract[] = {
#include "./precomputes/flange_wt_frac.dat"
};
error_t flange_init(flange *fl, p_buffer_t buf)
{
    //Duplicated constants, see pedal_precomputes.m
    fl->wt_size = 3000;
    fl->depth = 30;

    fl->op_buf = buf;
    fl->op_buf_sz = buf[0];

    fsaver_result fsr = no_error;
    fl->wt_data_int = plane_data_get_int(flange_wt_int, 0, &fsr);
    if(fsr)
        return ALGORITHM_INITIALIZATION_ERROR;

    fl->wt_data_fract = plane_data_get__Q15(flange_wt_fract, 0, &fsr);
    if(fsr)
        return ALGORITHM_INITIALIZATION_ERROR;

    fl->in_coef = Q15ftoi(0.3);
    fl->ff_coef = Q15ftoi(0.5);
    fl->fb_coef = Q15ftoi(0.3);

    fl->counter = 0;
    fl->wt_counter0 = 0;
    fl->wt_counter1 = 0;

    fl->fb_point = 0;

    return ERROR_OK;
}
error_t flange_set_params(void* dat, unsigned int num, unsigned int val)
{
    flange* fl = dat;

    if(val > MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    fl->ctr = val;

    return ERROR_OK;
}
error_t flange_process(void* dat, p_buffer_t in, p_buffer_t out)
{
    flange* fl = dat;

    _Q15 sample = *in;

    _Q15 p0 = _Q15add(sample, fl->fb_point);
    _Q15 p1 = Q15mpy(p0, fl->in_coef);
    _Q15 p2 = Q15mpy(fl->ff_coef, li_delay_line(sample, fl->op_buf,
            &fl->counter,fl->depth, fl->wt_data_int[fl->wt_counter1],
            fl->wt_data_fract[fl->wt_counter1]));
    
    sample = _Q15add(p1,p2);

    fl->fb_point = Q15mpy(fl->fb_coef, 
            delay_line_tap(fl->depth/2, &fl->op_buf[0], fl->counter, fl->depth));

    fl->wt_counter0++;
    if (fl->wt_counter0 >= (1 + (MAX_PARAMETER_VAL - fl->ctr)))
    {
        fl->wt_counter1++;
        if (fl->wt_counter1 >= fl->wt_size)
            fl->wt_counter1 = 0;
        fl->wt_counter0 = 0;
    }

    *out =  sample;

    return ERROR_OK;
}

error_t tremolo_init(tremolo *tr, p_buffer_t buf)
{
    tr->tremolo_counter = 0;
    tr->freq = 0;
    return ERROR_OK;
}
error_t tremolo_set_params(void* dat, unsigned int num, unsigned int val)
{
    tremolo* hp = dat;

    if(val > MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    hp->freq = val;

    return ERROR_OK;
}
error_t tremolo_process(void* dat, p_buffer_t in, p_buffer_t out)
{
    tremolo *tr = dat;

    _Q15 sample = *in;

    _Q15 lfo = _Q15sinPI(tr->tremolo_counter);
    sample = Q15mpy(sample, lfo);

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

    dl->ff_coef = Q15ftoi(0.5);
    dl->tap_coef = Q15ftoi(0.5);

    dl->buf = buf;
    dl->buf_len = buf[0];

    return ERROR_OK;
}
error_t delay_set_params(void* dat, unsigned int num, unsigned int val)
{
    delay* dl = dat;

    if(val > MAX_PARAMETER_VAL)
        return PARAMS_SET_ERROR;

    dl->time = val;

    return ERROR_OK;
}
error_t delay_process(void* dat, p_buffer_t in, p_buffer_t out)
{
    delay *dl = dat;

    _Q15 sample = *in;
   
    sample = _Q15add(Q15mpy(dl->ff_coef, sample),
            Q15mpy(dl->tap_coef, //Add one to parameter to avoid delay line with zero length
            delay_line(sample, dl->buf, &dl->counter, (dl->buf_len/11)*dl->time + 1)));

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
    ech->fb_coef = Q15ftoi(0.5);
    ech->tap_coef = Q15ftoi(0.3);

    return ERROR_OK;
}
error_t echo_set_params(void* dat, unsigned int num, unsigned int val)
{
    echo* ech = dat;

    if(val > MAX_PARAMETER_VAL)
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

    rev->ap_coefs[0] = Q15ftoi(0.35);
    rev->ap_coefs[1] = Q15ftoi(0.27);

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

    if(val > MAX_PARAMETER_VAL)
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





