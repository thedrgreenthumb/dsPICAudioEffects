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

_Q15 dc_blocker(_Q15 sample);

typedef struct _bypass
{
    //Fake variable
    unsigned int i;
} bypass;
declare_algo_funcs(bypass)

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
    unsigned int ctr;
    _Q15 env_buf;

} compression;
declare_algo_funcs(compression)

//FILTERS !!!!!!!!!!!!!!!!!!!!!!!!!!
/*
typedef struct _lp_filter
{
    unsigned int freq;
    _Q16 filter_buf[4];

    error_t (*p_set_params)(unsigned int paramNumber, unsigned int val);
    error_t (*p_process)(p_buffer_t in, p_buffer_t out);
} lp_filter;
declare_algo_funcs(lp_filter)

typedef struct _bp_filter
{
    unsigned int freq;
    _Q16 filter_buf[4];

    error_t (*p_set_params)(unsigned int paramNumber, unsigned int val);
    error_t (*p_process)(p_buffer_t in, p_buffer_t out);
} bp_filter;
declare_algo_funcs(bp_filter)

typedef struct _hp_filter
{
    unsigned int freq;
    _Q16 filter_buf[4];

    error_t (*p_set_params)(unsigned int paramNumber, unsigned int val);
    error_t	(*p_process)(p_buffer_t in, p_buffer_t out);
} hp_filter;
declare_algo_funcs(hp_filter)

//MODULATORS

typedef struct _chorus
{
    unsigned  int ctr;
    _Q15* op_buf;
    unsigned int chorus_counters[6];
    _Q15 chorus_fb_point[2];

    error_t (*p_set_params)(unsigned int paramNumber, unsigned int val);
    error_t (*p_process)(p_buffer_t in, p_buffer_t out);
} chorus;
declare_algo_funcs(chorus)

typedef struct _flange
{
    unsigned  int ctr;
    _Q15* op_buf;
    unsigned int flange_counter;
    unsigned int flange_wt_counter0;
    unsigned int flange_wt_counter1;
    _Q15 flange_fb_point;

    error_t (*p_set_params)(unsigned int paramNumber, unsigned int val);
    error_t (*p_process)(p_buffer_t in, p_buffer_t out);
} flange;
declare_algo_funcs(flange)

typedef struct _tremolo
{
    unsigned  int freq;
    _Q15 tremolo_counter;

    error_t (*p_set_params)(unsigned int paramNumber, unsigned int val);
    error_t (*p_process)(p_buffer_t in, p_buffer_t out);
} tremolo;
declare_algo_funcs(tremolo)

//DELAYS!!!!!!!!!

typedef struct _delay
{
    unsigned  int time;
    unsigned int delay_counter;

    error_t (*p_set_params)(unsigned int paramNumber, unsigned int val);
    error_t (*p_process)(p_buffer_t in, p_buffer_t out);
} delay;
declare_algo_funcs(delay)

typedef struct _echo
{
    unsigned  int time;
    _Q15 echo_fb_point;
    unsigned int echo_counter;

    error_t (*p_set_params)(unsigned int paramNumber, unsigned int val);
    error_t (*p_process)(p_buffer_t in, p_buffer_t out);
} echo;
declare_algo_funcs(echo)

typedef struct _reverb
{
    unsigned  int rt;
    _Q15 reverb_fb_p;
    unsigned int reverb_counter[3];

    error_t (*p_set_params)(unsigned int paramNumber, unsigned int val);
    error_t (*p_process)(p_buffer_t in, p_buffer_t out);
} reverb;
declare_algo_funcs(reverb)
*/




