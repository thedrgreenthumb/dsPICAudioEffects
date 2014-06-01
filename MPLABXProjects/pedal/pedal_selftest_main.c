/**********************************************************************
* FileName:        main.c
* FileVersion      1.01
* Processor:       dsPIC33FJ128GP802
* Compiler:        X16 v1.20
*
* Design in:       SAL
* Design by:
* Feedback:
*
* Project:         dsPIC33 Audio Effects Pedal project.
* ProjectVersion:  1.01 
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

    //Test loop
    sample=ADC1BUF0;
    out_sample_L = sample;
    out_sample_R = sample;
  
    DAC1LDAT=out_sample_L;

    IFS4bits.DAC1LIF = 0; // Clear Left Channel Interrupt Flag
}

void __attribute__((interrupt, no_auto_psv))_DAC1RInterrupt(void)
{
    DAC1RDAT=out_sample_R;

    IFS4bits.DAC1RIF = 0; // Clear Right Channel Interrupt Flag
}

void delay(unsigned long int i)
{
    volatile unsigned long int j = 0;
    for(j = 0; j < i; j++)
        ;
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

    //HW initialization
    adc_init();
    audio_dac_init();
    pins_init();

    TRISB = 0;

    int is_button_indication = 0;
    int i = 0;
    int j = 0;
    while(1)
    {
        delay(1000000);
        is_button_indication = 0;
        //If any button pressed, indicate it
        for(i = 0; i < 4; i++)
            if(is_button_pressed(i))
            {
                seven_sigm_indicate_num(i);
                is_button_indication = 1;
            }

        //Indicate numbers from 0 to 9
        if(!is_button_indication)
        {
            seven_sigm_indicate_num(j);
            j++;
            if(j > 9)
                j = 0;
        }
    };
    
    return 0;
}



