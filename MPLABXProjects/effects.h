/**********************************************************************
* FileName:        Effects.h
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
#include "runner/runner.h"

#define declare_algo_funcs(NAME) \
error_t NAME##_init(NAME *data, p_buffer_t buf); \
error_t NAME##_set_params(void *dat, unsigned int num, unsigned int val); \
error_t NAME##_process(void *dat, p_buffer_t in, p_buffer_t out);

_Q15 w_dc_blocker(_Q15 sample);

typedef struct _bypass
{
    //Fake variable
    unsigned int i;
} bypass;
declare_algo_funcs(bypass)


//Amplitude based
typedef struct _hard_clipping
{
    _Q16 gain;
    const _Q16* filter_coefs;
    _Q16 filter_buf[4];
} hard_clipping;
declare_algo_funcs(hard_clipping)

typedef struct _soft_clipping
{
    _Q16 gain;
    const _Q16* filter_coefs;
    _Q16 filter_buf[4];

} soft_clipping;
declare_algo_funcs(soft_clipping)

typedef struct _compression
{
    _Q16 ctr;
} compression;
declare_algo_funcs(compression)

//Filtering
typedef struct _lp_filter
{
    unsigned int freq;
    _Q16 filter_buf[4];
} lp_filter;
declare_algo_funcs(lp_filter)

typedef struct _bp_filter
{
    unsigned int freq;
    _Q16 gain_coef;
    _Q16 filter_buf[4];
} bp_filter;
declare_algo_funcs(bp_filter)

typedef struct _hp_filter
{
    unsigned int freq;
    _Q16 filter_buf[4];
} hp_filter;
declare_algo_funcs(hp_filter)

//Modulation
typedef struct _chorus
{
    unsigned  int ctr;

    _Q15* op_buf;
    int op_buf_sz;

    _Q15* dl1_buf;
    _Q15* dl2_buf;
    int dl1_buf_sz;
    int dl2_buf_sz;

    //Wave table
    const int* w_int_part;
    const _Q15* w_frac_part;
    unsigned int wt_size;
    unsigned int depth;

    unsigned int counters[6];

    _Q15 in_coefs[2];
    _Q15 fb_coefs[2];
    _Q15 ff_coefs[2];

    unsigned int tap_szs[2];

    _Q15 fb_points[2];
} chorus;
declare_algo_funcs(chorus)

typedef struct _flange
{
    unsigned int ctr;

    _Q15* op_buf;
    unsigned int op_buf_sz;

    const int* wt_data_int;
    const _Q15* wt_data_fract;
    unsigned int wt_size;
    unsigned int depth;

    _Q15 in_coef;
    _Q15 ff_coef;
    _Q15 fb_coef;

    unsigned int counter;
    unsigned int wt_counter0;
    unsigned int wt_counter1;

    _Q15 fb_point;
} flange;
declare_algo_funcs(flange)

typedef struct _tremolo
{
    unsigned int freq;
    _Q15 tremolo_counter;
} tremolo;
declare_algo_funcs(tremolo)

//Delay effects
typedef struct _delay
{
    unsigned int time;

    _Q15* buf;
    unsigned int buf_len;

    unsigned int counter;

    _Q15 ff_coef;
    _Q15 tap_coef;
} delay;
declare_algo_funcs(delay)

typedef struct _echo
{
    unsigned  int time;

    _Q15* buf;
    unsigned int counter;
    unsigned int buf_len;

    _Q15 fb_point;

    _Q15 ff_coef;
    _Q15 fb_coef;
    _Q15 tap_coef;
    
} echo;
declare_algo_funcs(echo)

typedef struct _reverb
{
    unsigned int rt;

    _Q15* buf;
    unsigned int buf_len;

    _Q15 fb_point;

    unsigned int early_counter;
    unsigned int ap_counters[2];

    //Initialization data
    _Q15 ff_coef;
    _Q15 fb_coef;

    _Q15 tap_coefs[4];
    unsigned int tap_sizes[3];
    _Q15 ap_coefs[2];
    unsigned int ap_sizes[2];
} reverb;
declare_algo_funcs(reverb)





