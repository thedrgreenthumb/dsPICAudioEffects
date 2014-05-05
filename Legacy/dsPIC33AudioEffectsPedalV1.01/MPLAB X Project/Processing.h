/**********************************************************************
* FileName:        Processing.h
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

void controls_processing(void);
void samples_processing(_Q15 input_sample, _Q15* out_sample_L, _Q15* out_sample_R);
