/**********************************************************************
* FileName:        main.c
* FileVersion      1.01
* Processor:       dsPIC33FJ128GP802
* Compiler:        C30 v3.21
*
* Design in:       SAL
* Design by:
* Feedback:
*
* Project:         dsPIC33 Audio Effects Pedal project.
* ProjectVersion:  1.01 
**********************************************************************/

#include <p33FJ128GP802.h>

#include <libq.h>

#include "Initialization.h"
#include "Processing.h"

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

    DAC1LDAT=out_sample_L;

    IFS4bits.DAC1LIF = 0; // Clear Left Channel Interrupt Flag
}

void __attribute__((interrupt, no_auto_psv))_DAC1RInterrupt(void)
{
    DAC1RDAT=out_sample_R;

    IFS4bits.DAC1RIF = 0; // Clear Right Channel Interrupt Flag
}

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

    //DSC's peripherial configuration
    adc_init();
    audio_dac_init();
    pins_init();

    while(1){};
}



