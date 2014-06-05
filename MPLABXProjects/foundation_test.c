/**********************************************************************
* FileName:        main.c
* FileVersion      1.01
* Processor:       dsPIC33FJ128GP802
* Compiler:        MPLAB C30 v3.31
*
* License:         MIT
*
* Design in:       SAL
* Design by:
* Feedback:
*
* Project:         dsPIC33 Audio Effects Pedal project.
* ProjectVersion:  0.01
**********************************************************************/

#include <p33FJ128GP802.h>

#include <stdio.h>
#include <libq.h>
#include "foundation.h"

//Configuration bits setup
_FGS(GWRP_OFF & GCP_OFF);
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE);
_FWDT(FWDTEN_OFF);

//Test functions,
//run simulator and see UART 1 Output
int test_Q15mpy(float verification_err, int do_print);
int test_Q16mpy(float verification_err, int do_print);
int test_Q15andQ16_conversions(float verification_err, int do_print);
int test_Q16toi(float verification_err, int do_print);

inline int verify(float expected, float got, float error) {
    if((expected - error <= got) && (got <= expected + error))
        return 1;
    else
        return 0;
}

int main(void)
{

    /* Configure Oscillator to operate the device at 40MHz.
	 * Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
	 * Fosc= 7.37M*40/(2*2)=80Mhz for 7.37M input clock */

    //Fosc=79227500, Fcy=39613750
    PLLFBD=41;			     	/* M=43	*/
    CLKDIVbits.PLLPOST=0;		/* N1=2	*/
    CLKDIVbits.PLLPRE=0;		/* N2=2	*/
    OSCTUN=0;

    __builtin_write_OSCCONH(0x01);      /*	Initiate Clock Switch to FRC with PLL*/
    __builtin_write_OSCCONL(0x01);
    while (OSCCONbits.COSC != 0b01);	/*	Wait for Clock switch to occur	*/
    while(!OSCCONbits.LOCK);

    int intermid_print = 0;
    
    int test_state = 0;
    test_state += test_Q15mpy(0.1, intermid_print);
    test_state += test_Q16mpy(1.0, intermid_print);
    test_state += test_Q15andQ16_conversions(1.0, intermid_print);
    test_state += test_Q16toi(1.0, intermid_print);

    if (test_state)
        printf("Tests FAILED.\n");
    else
        printf("Tests PASSED.\n");
}

int test_Q15mpy(float verification_err, int do_print)
{
    printf("Test Q15 multiplication : Q15mpy()\n");
    if(do_print)
        printf("expected : got = a * b\n");

    unsigned int data_size = 10;
    float data1[10] = {0,  0.1, 0.5,  0.25,  0.99,     0,  0.5, 0.8,  1, 1};
    float data2[10] = {0,-0.99, 0.5, -0.25, -0.99, -0.99, -0.5,-0.3,  1, -1};
    
    unsigned int i = 0;
    float test_result = 0;
    float expected_result = 0;
    for(i = 0; i < data_size; i++)
    {
        expected_result = data1[i] * data2[i];
        test_result = _itofQ15(Q15mpy(_Q15ftoi(data1[i]),
                _Q15ftoi(data2[i])));

        if(!verify(expected_result, test_result, verification_err))
            return 1;

        if(do_print)
            printf("%f : %f = %f * %f\n",expected_result,test_result,
                    data1[i], data2[i]);
    }

    return 0;
}

int test_Q16mpy(float verification_err, int do_print)
{
    printf("Test Q16 multiplication : Q16mpy()\n");
    if(do_print)
        printf("expected : got = a * b\n");

    unsigned int data_size = 10;
    //The Q16 format provide ability to manipulate values fro -32767 to 32767
    float data1[10] = {0,  0.1,  15,  250,  128,     0,  0.5,  0.8, 0.9,  32000};
    float data2[10] = {0, -0.1, -15, -0.1,    2,  -0.1, -0.5, -0.3, -32767, 1.01};

    unsigned int i = 0;
    float test_result = 0;
    float expected_result = 0;
    for(i = 0; i < data_size; i++)
    {
        expected_result = data1[i] * data2[i];
        test_result = _itofQ16(Q16mpy(_Q16ftoi(data1[i]),
                _Q16ftoi(data2[i])));

        if(!verify(expected_result, test_result, verification_err))
            return 1;

        if(do_print)
            printf("%f : %f = %f * %f\n",expected_result,test_result,
                    data1[i], data2[i]);
    }

    return 0;
}

int test_Q15andQ16_conversions(float verification_err, int do_print)
{
    printf("Test Q15 with Q16 conversions : Q15toQ16() and Q16toQ15() \n");
    if(do_print)
        printf("expected : 15to16 : 16to15\n");

    unsigned int data_size = 10;
    float data1[10] = {0,  -0.1,  -0.15,  0.25,  -0.75,  -0.99,  0.1,  -0.5, 0.99, 0.33};

    unsigned int i = 0;
    float expected_result = 0;
    float test_15to16 = 0;
    float test_16to15 = 0;
    for(i = 0; i < data_size; i++)
    {
        expected_result = data1[i];
        test_15to16 = _itofQ16(Q15toQ16(_Q15ftoi(data1[i])));
        test_16to15 = _itofQ15(Q16toQ15sat(_Q16ftoi(data1[i])));

        //Check average here
        if(!verify(expected_result, (test_15to16 + test_16to15)*0.5, verification_err))
            return 1;

        if(do_print)
            printf("%f : %f : %f\n",expected_result,test_15to16,test_16to15);
    }

    return 0;
}

int test_Q16toi(float verification_err, int do_print)
{
    printf("Test Q16 to integer conversion : Q16toi()\n");
    if(do_print)
        printf("expected : 16toi\n");

    unsigned int data_size = 10;
    float data1[10] = {0,  -0.1,  0.75,  1000.36,  -5723.0,  413.27, -863.87,  1298.15, -15256.36, 15381};

    unsigned int i = 0;
    int expected_result = 0;
    int test_result = 0;
    for(i = 0; i < data_size; i++)
    {
        expected_result = (int)data1[i];
        test_result = Q16toi(_Q16ftoi(data1[i]));

        if(do_print)
            printf("%d : %d\n",expected_result,test_result);

        if(!verify(expected_result, test_result, verification_err))
            return 1;
    }

    return 0;
}
