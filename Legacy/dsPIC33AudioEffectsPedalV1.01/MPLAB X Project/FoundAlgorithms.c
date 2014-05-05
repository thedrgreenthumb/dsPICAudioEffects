/**********************************************************************
* FileName:        FoundAlgorithms.c
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

inline _Q15 Q15mpy(_Q15 a, _Q15 b)
{
    signed int result = 0;
    register int accA asm("A");

    accA = __builtin_mpy(a, b, 0, 0, 0, 0, 0, 0);
    result = __builtin_sac(accA ,0);

    return(result);
}

inline _Q16 Q16abs(_Q16 a)
{
    _Q16 b = 0;

    if(a >= 0)
        b = a;
    else
        b = _Q16neg(a);

    return b;
}

inline _Q16 Q16mpy(_Q16 a, _Q16 b)
{
    _Q16 c = 0;

    if((a < 0) && (b >= 0))
    {
        a = _Q16neg(a);
        c = _Q16mac(a, b, 0);
        c = _Q16neg(c);
    }
    else
    if((a >= 0) && (b < 0))
    {
        b = _Q16neg(b);
        c = _Q16mac(a, b, 0);
        c = _Q16neg(c);
    }
    else
    if((a < 0) && (b < 0))
    {
        a = _Q16neg(a);
        b = _Q16neg(b);
        c = _Q16mac(a, b, 0);
    }
    else
        c = _Q16mac(a, b, 0);

    return c;
}

inline _Q16 Q16pow(_Q16 x, _Q16 p)
{
    _Q16 y = _Q16power(x, p);

    return y;
}

inline int Q16toi(_Q16 x)
{
    int y;

    x = x >> 16;
    y = x;

    return y;
}
inline _Q16 Q15toQ16(_Q15 x)
{
   _Q16 y;

   y = x;
   y = y << 1;

   return y;
}

inline _Q15 Q16toQ15(_Q16 x)
{
   _Q15 y;

    y = x >> 1;

   return y;
}

inline _Q15 Q15ftoi(float x)
{
    return _Q15ftoi(x);
}

inline _Q16 Q16ftoi(float x)
{
    return _Q16ftoi(x);
}

inline _Q15 Q15sinPI(_Q15 x)
{
    return _Q15sinPI(x);
}

_Q15 li_delay_line(_Q15 sample, _Q15 *buf, unsigned int* counter, int length, int intVal, _Q15 fracVal)
{
    _Q15 out = 0;
    int counter_val = *counter;
    int wrap_adr = counter_val - intVal;

    if(wrap_adr > 0)
            out = Q15mpy(buf[wrap_adr - 1],fracVal) + Q15mpy(buf[wrap_adr],(32767 - fracVal));
    else if(wrap_adr == 0)
            out = Q15mpy(buf[length - 1], fracVal) + Q15mpy(buf[0],(32767 - fracVal));
    else if(wrap_adr < 0)
    {
            wrap_adr = length + wrap_adr;
            out = Q15mpy(buf[wrap_adr - 1],fracVal) + Q15mpy(buf[wrap_adr],(32767 - fracVal));
    }

    counter_val++;
    if(counter_val >= length)
            counter_val = 0;

    buf[counter_val] = sample;

    *counter = counter_val;

    return(out);
}

_Q15 delay_line(_Q15 sample, _Q15 *buf, unsigned int *counter, unsigned int length)
{
    int counter_val;
    _Q15 out;

    counter_val = (int)*counter;

    out = *(buf + counter_val);
    *(buf+counter_val) = sample;
    counter_val++;
    if (counter_val > length-1)
            counter_val = 0;

    *counter = (unsigned int)counter_val;

    return out;
}

_Q15 delay_line_tap(unsigned int read_position, _Q15* buf, unsigned int counter, unsigned int length)
{
    _Q15 out;

    int wrap_adr = counter - read_position;

    if(wrap_adr > 0)
            out = buf[wrap_adr];
    else if(wrap_adr == 0)
            out = buf[0];
    else if(wrap_adr < 0)
    {
            wrap_adr = length + wrap_adr;
            out = buf[wrap_adr];
    }

    return out;
}

_Q15 all_pass(_Q15 sample, _Q15 coef, _Q15 *buf, unsigned int *count, unsigned int length)
{
    _Q15 p0 = 0;
    _Q15 out = 0;

    p0 = sample +
        Q15mpy(_Q15neg(coef),delay_line(sample, buf, count, length));
    out = Q15mpy(p0,coef) + delay_line(sample, buf, count, length);

    return(out);
}

_Q16 DF2SOStructure(_Q16 in, _Q16 *numPtr, _Q16 *denumPtr, _Q16 *xh, _Q16 *yh)
{
    _Q16 out;

    out = Q16mpy(numPtr[0],in) + Q16mpy(numPtr[1],xh[0]) + Q16mpy(numPtr[2],xh[1]) - Q16mpy(denumPtr[1],yh[0]) - Q16mpy(denumPtr[2], yh[1]);
    xh[1] = xh[0];
    xh[0] = in;
    yh[1] = yh[0];
    yh[0] = out;

    return(out);
}

