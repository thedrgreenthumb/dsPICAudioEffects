/**********************************************************************
* FileName:        Algorithms.c
* FileVersion      1.01
* Dependencies:    Algorithms.h
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
#include "Algorithms.h"

_Q15 HardClipping(_Q15 sample, int ctrVal)
{
    //Positive component clipping
    if (sample > ctrVal)
        sample = ctrVal;
    //Negative component clipping
    if (sample < _Q15neg(ctrVal))
        sample = _Q15neg(ctrVal);

    return(sample);
}

_Q15 OnePoleFilter(_Q15 sample, _Q15 coef, _Q15 *buf)
{
    //y(n)=x(n)*coef + y(n-1)*(1-coef)
    if (coef>0)
        sample=Q15mpy(sample,coef)+Q15mpy(*buf,32767-coef);
    *buf=sample;

    return(sample);
}

_Q15 AllPassWithDelayLine(_Q15 sample, _Q15 *buf, unsigned int *count, unsigned int length, _Q15 coef)
{
    _Q15 p0 = 0;
    _Q15 delayLineOut = DelayLine(sample, buf, count, length);
    _Q15 out = 0;

    p0 = sample +
        Q15mpy(_Q15neg(coef),delayLineOut);
    out = Q15mpy(p0,coef) + delayLineOut;

    return(out);
}

_Q15 DelayLine(_Q15 sample, _Q15 *buf, unsigned int *counter, unsigned int length)
{
    unsigned int counterVal = *counter;
    _Q15 out;

    out =buf[counterVal];
    buf[counterVal] = sample;
    counterVal++;
    if(counterVal >= length)
        counterVal = 0;

    *counter = counterVal;

    return(out);
}

_Q15 LInterpDelLine(_Q15 sample, _Q15 *buf, int length, int intVal, _Q15 fracVal)
{
     int internalCounter;
     _Q15 out;

     //Linear interpolation calcualtion: y(n)= x(n)*(1 - frac) + x(n+1)*frac.
     out = Q15mpy(buf[intVal],32767-fracVal)+ Q15mpy(buf[intVal+1],fracVal);
     //FIFO update
     for(internalCounter = length-1;internalCounter > 0;internalCounter--)
        buf[internalCounter] = buf[internalCounter-1];
     buf[0] = sample;

     return(out);
}

//Additional
inline _Q15 Q15mpy(_Q15 a, _Q15 b)
{

    signed int result = 0;
    register int accA asm("A");

    accA = __builtin_mpy(a, b, 0, 0, 0, 0, 0, 0);
    result = __builtin_sac(accA ,0);

    return(result);
}

void Delay(unsigned long int d)
{
    int i=0;
    while (i<d)
    {
        i++;
        asm volatile ("NOP");
    }
}
