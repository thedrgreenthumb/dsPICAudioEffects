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

#define declare_algo_funcs(NAME) \
error_t NAME##_init(NAME* bp, pBuffer_t buf, initialization_t identificationType); \
error_t NAME##_set_params(void* str, paramNum_t paramNumber, unsigned int val); \
error_t NAME##_process(void* str, pBuffer_t in, pBuffer_t out);

_Q15 dc_blocker(_Q15 sample);

typedef struct _bypass
{
	//Functions pointers
	error_t (*p_set_params)(void* str, paramNum_t paramNumber, unsigned int val);
	error_t	(*p_process)(void* str, pBuffer_t in, pBuffer_t out);
} bypass;
declare_algo_funcs(bypass)

typedef struct _hard_clipping
{
	unsigned int gain;
	_Q16 filter_buf[4];

	//Functions pointers
	error_t (*p_set_params)(void* str, paramNum_t paramNumber, float val);
	error_t	(*p_process)(void* str, pBuffer_t in, pBuffer_t out);
} hard_clipping;
declare_algo_funcs(hard_clipping)

typedef struct _soft_clipping
{
	unsigned int gain;
	_Q16 filter_buf[4];

	//Functions pointers
	error_t (*p_set_params)(void* str, paramNum_t paramNumber, float val);
	error_t	(*p_process)(void* str, pBuffer_t in, pBuffer_t out);
} soft_clipping;
declare_algo_funcs(soft_clipping)

typedef struct _compression
{
	//Add appropriate params, must be totally rewrited 

	//Functions pointers
	error_t (*p_set_params)(void* str, paramNum_t paramNumber, float val);
	error_t	(*p_process)(void* str, pBuffer_t in, pBuffer_t out);
} compression;
declare_algo_funcs(compression)

//FILTERS !!!!!!!!!!!!!!!!!!!!!!!!!!

typedef struct _lp_filter
{
	usnigned int control;
	_Q16 filter_buf[4];

	//Functions pointers
	error_t (*p_set_params)(void* str, paramNum_t paramNumber, float val);
	error_t	(*p_process)(void* str, pBuffer_t in, pBuffer_t out);
} lp_filter;
declare_algo_funcs(lp_filter)

typedef struct _bp_filter
{
	usnigned int control;
	_Q16 filter_buf[4];

	//Functions pointers
	error_t (*p_set_params)(void* str, paramNum_t paramNumber, float val);
	error_t	(*p_process)(void* str, pBuffer_t in, pBuffer_t out);
} bp_filter;
declare_algo_funcs(bp_filter)

typedef struct _hp_filter
{
	usnigned int control;
	_Q16 filter_buf[4];

	//Functions pointers
	error_t (*p_set_params)(void* str, paramNum_t paramNumber, float val);
	error_t	(*p_process)(void* str, pBuffer_t in, pBuffer_t out);
} hp_filter;
declare_algo_funcs(hp_filter)

//MODULATORS

typedef struct _chorus
{
	usnigned int control;
	unsigned int chorus_counters[6];
	_Q15 chorus_fb_point[2];

	//Functions pointers
	error_t (*p_set_params)(void* str, paramNum_t paramNumber, float val);
	error_t	(*p_process)(void* str, pBuffer_t in, pBuffer_t out);
} chorus;
declare_algo_funcs(chorus)

typedef struct _flange
{
	usnigned int control;
	unsigned int flange_counter;
	unsigned int flange_wt_counter0;
	unsigned int flange_wt_counter1;
	_Q15 flange_fb_point;

	//Functions pointers
	error_t (*p_set_params)(void* str, paramNum_t paramNumber, float val);
	error_t	(*p_process)(void* str, pBuffer_t in, pBuffer_t out);
} flange;
declare_algo_funcs(flange)

typedef struct _tremolo
{
	usnigned int control;
	_Q15 tremolo_counter;

	//Functions pointers
	error_t (*p_set_params)(void* str, paramNum_t paramNumber, float val);
	error_t	(*p_process)(void* str, pBuffer_t in, pBuffer_t out);
} tremolo;
declare_algo_funcs(tremolo)

//DELAYS!!!!!!!!!

typedef struct _delay
{
	usnigned int control;
	unsigned int delay_counter;

	//Functions pointers
	error_t (*p_set_params)(void* str, paramNum_t paramNumber, float val);
	error_t	(*p_process)(void* str, pBuffer_t in, pBuffer_t out);
} delay;
declare_algo_funcs(delay)

typedef struct _echo
{
	usnigned int control;
	_Q15 echo_fb_point;
	unsigned int echo_counter;

	//Functions pointers
	error_t (*p_set_params)(void* str, paramNum_t paramNumber, float val);
	error_t	(*p_process)(void* str, pBuffer_t in, pBuffer_t out);
} echo;
declare_algo_funcs(echo)

typedef struct _reverb
{
	usnigned int control;
	_Q15 reverb_fb_p;
	unsigned int reverb_counter[3];

	//Functions pointers
	error_t (*p_set_params)(void* str, paramNum_t paramNumber, float val);
	error_t	(*p_process)(void* str, pBuffer_t in, pBuffer_t out);
} reverb;
declare_algo_funcs(reverb)

void signal_fork(_Q15 sample, _Q15* out_sample_L, _Q15* out_sample_R);



