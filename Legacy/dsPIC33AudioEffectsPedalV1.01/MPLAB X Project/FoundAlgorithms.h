/**********************************************************************
* FileName:        FoundAlgorithms.h
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

inline _Q15 Q15mpy(_Q15 a, _Q15 b);
inline int Q16toi(_Q16 x);
inline _Q16 Q15toQ16(_Q15 x);
inline _Q15 Q16toQ15(_Q16 x);
inline _Q16 Q16abs(_Q16 a);
inline _Q16 Q16pow(_Q16 x, _Q16 p);
inline _Q16 Q16mpy(_Q16 x, _Q16 y);
inline _Q15 Q15ftoi(float x);
inline _Q16 Q16ftoi(float x);
inline _Q15 Q15sinPI(_Q15 x);
_Q15 li_delay_line(_Q15 sample, _Q15 *buf, unsigned int* counter, int length, int intVal, _Q15 fracVal);
_Q15 delay_line(_Q15 sample, _Q15 *buf, unsigned int *counter, unsigned int length);
_Q15 delay_line_tap(unsigned int read_position, _Q15* buf, unsigned int counter, unsigned int length);
_Q15 all_pass(_Q15 sample, _Q15 coef, _Q15 *buf, unsigned int *count, unsigned int length);
_Q16 DF2SOStructure(_Q16 in, _Q16 *numPtr, _Q16 *denumPtr, _Q16 *xh, _Q16 *yh);
