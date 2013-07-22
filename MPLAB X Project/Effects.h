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

#define MAX_PARAMETER_VAL 9

_Q15 dc_blocker(_Q15 sample);

_Q15 hard_clipping(_Q15 sample, unsigned int parameter_val);
_Q15 soft_clipping(_Q15 sample, unsigned int parameter_val);
_Q15 compression(_Q15 sample, unsigned int parameter_val);

_Q15 lp_filter(_Q15 sample, unsigned int parameter_val);
_Q15 bp_filter(_Q15 sample, unsigned int parameter_val);
_Q15 hp_filter(_Q15 sample, unsigned int parameter_val);

_Q15 chorus(_Q15 sample, unsigned int parameter_val);
_Q15 flange(_Q15 sample, unsigned int parameter_val);
_Q15 tremolo(_Q15 sample, unsigned int parameter_val);

_Q15 delay(_Q15 sample, unsigned int parameter_val);
_Q15 echo(_Q15 sample, unsigned int parameter_val);
_Q15 reverb(_Q15 sample, unsigned int parameter_val);

void panoram_enhancer(_Q15 sample, _Q15* out_sample_L, _Q15* out_sample_R);



