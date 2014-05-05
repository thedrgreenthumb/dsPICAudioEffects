/**********************************************************************
* FileName:        Algorithms.h
* FileVersion      1.01
* Dependencies:
* Processor:       dsPIC33FJ256GP506
* Compiler:        C30 v3.21
*
* Design in:       SAL
* Design by:
* Feedback:
*
* Project:         dsPIC DSC Starter Kit 1 Audio Effects Project.
* ProjectVersion:  1.01
**********************************************************************/
#include <libq.h>

//Nonlinear processing functions
_Q15 HardClipping(_Q15 sample, int ctrVal);

//Filtering
_Q15 OnePoleFilter(_Q15 sample, _Q15 coef, _Q15 *buf);
_Q15 AllPassWithDelayLine(_Q15 sample, _Q15 *buf, unsigned int *count, unsigned int length, _Q15 coef);

//Delay lines
_Q15 DelayLine(_Q15 sample, _Q15 *buf, unsigned int *counter, unsigned int length);
_Q15 LInterpDelLine(_Q15 sample, _Q15 *buf, int length, int intVal, _Q15 fracVal);

//Additional
inline _Q15 Q15mpy(_Q15 a, _Q15 b);
void Delay(unsigned long int d);
