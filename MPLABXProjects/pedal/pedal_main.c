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

#include "pedal_initialization.h"
#include "pedal_processing.h"

//Configuration bits setup
_FGS(GWRP_OFF & GCP_OFF);
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE);
_FWDT(FWDTEN_OFF);

_Q15 sample;
_Q15 out_sample_L;
_Q15 out_sample_R;

//Audio DAC interrupt, sampling frequency = 19,3426513671875 KHz
void __attribute__((interrupt, no_auto_psv))_DAC1LInterrupt(void)
{
    AD1CON1bits.SAMP = 0; // Start next ADC convertion

    sample=ADC1BUF0;

    controls_processing();
    samples_processing(sample, &out_sample_L, &out_sample_R);

    //!!!Here I had a problem with  Left channel of Audio DAC,
    //the analog signal from here was hissed,
    //so pass output thru right DAC channel
    DAC1LDAT = 0;

    IFS4bits.DAC1LIF = 0; // Clear Left Channel Interrupt Flag
}

void __attribute__((interrupt, no_auto_psv))_DAC1RInterrupt(void)
{
    DAC1RDAT=out_sample_R;

    IFS4bits.DAC1RIF = 0; // Clear Right Channel Interrupt Flag
}

//Global algorithms buffers for runners
#define MOD_MEMORY_SIZE (600)
_Q15 mod_buffer[MOD_MEMORY_SIZE];
#define DELAYS_MEMORY_SIZE (7000)
_Q15 delays_buffer[DELAYS_MEMORY_SIZE];

int main(void){

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

    //Effects initialization
    if(runners_init(mod_buffer, MOD_MEMORY_SIZE, delays_buffer, DELAYS_MEMORY_SIZE))
    {
        //Indicate error
        seven_sigm_indicate(E_CHARACTER);
        //Forbid interrupts
        SR = SR | 0x00E0;
    }

    //HW initialization
    adc_init();
    audio_dac_init();
    pins_init();

    while(1){}

    /*
    //Input data
    float in_data[12] = {1, 1, 1, -1, -1, -1, 1, 1, -1, -1, 1, -1};
    _Q15 input[12];
    int i = 0;
    for(i = 0; i < 12; i++) input[i] = Q15ftoi(in_data[i]);


    _Q15 out1[12];
    _Q15 out2;
    for(i = 0; i < 12; i++)
    {

        samples_processing(input[i], &out1[i], &out2);

    };

    float out[12];
    for(i = 0; i < 12;)
    {
        out[i] = _itofQ15(out1[i]);
        i++;
    }
    */
    return 0;
}



