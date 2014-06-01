/**********************************************************************
* FileName:        main.�
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

/*Simplified structure of main.c*****************************************
*
*
*Different includes
*
*Configuration bits setup
*
*Algorithms control variables and interrupt functions
*
*Algorithm global variables and constants declaration
*
*DCI interrupt (audio processing here)
*
*main function
*
*
****************************************************************************/
//Math
#include <math.h>
#include <libq.h>
//Board drivers
#include "./sk1_Initializtion.h"
#include "WM8510CodecDrv.h"

#include "../runner/runner.h"

//Configuration bits
_FGS(GWRP_OFF & GCP_OFF);
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE);
_FWDT(FWDTEN_OFF);

//Processing control variables
//Algorithm number
unsigned int ef_num = 0;
//Algorithm control value
unsigned int ef_ctr = 0;

//Button S1, set effect number and indicate on LED's
void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void)
{
    ef_ctr = 0;
    ef_num++;
    if(ef_num > 7)
        ef_num = 0;

    LATC = (~ef_num) << 13;
           
    IFS1bits.INT1IF = 0;
}

//Button S2, change effect param
void __attribute__((interrupt, no_auto_psv)) _INT2Interrupt(void)
{
    //Button S2
    if (PORTDbits.RD9 == 0)
    {
        ef_ctr++;
        if(ef_ctr > 9)
            ef_ctr = 0;
    }

    IFS1bits.INT2IF = 0;
}

extern runner_t runner;
//DCI interrupt (sample by sample processing)
void __attribute__((__interrupt__,no_auto_psv)) _DCIInterrupt(void)
{
    //Input sample comes in Q15
    _Q15 sample = RXBUF0;

    runner_set_effect_num(&runner, ef_num);
    runner_set_param(&runner, 0, ef_ctr);
    runner_sbs_process(&runner, &sample, &sample);
	
    TXBUF0 = sample;
    //Output in Q15

    // Close interrupt flag
    IFS3bits.DCIIF = 0;
}					

//Global algorithms buffer for runners
#define EFFECTS_MEMORY_SIZE (7000)
_Q15 algorithms_buffer[EFFECTS_MEMORY_SIZE];
unsigned int mod_buf_size[] = {
#include "../precomputes/mod_effects_buf_sz.dat"
};

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

    __builtin_write_OSCCONH(0x01);	/*	Initiate Clock Switch to FRC with PLL*/
    __builtin_write_OSCCONL(0x01);
    while (OSCCONbits.COSC != 0b01);	/*	Wait for Clock switch to occur	*/
    while(!OSCCONbits.LOCK);

    //Ext. int 1
    IPC5=0b0000000000000010;
    //Ext. int 2 
    IPC7=0b0000001100100000;
    //Codec initialization
    WM8510Init();
    WM8510Start();
    WM8510SampleRate16KConfig();
    
    //Output amplifier control pins initialization
    LM4811PinsInit();
    LM4811SetVolDOWN(32); //Set Volume of LM4811 to min value
    InitLEDsAndBUTs();
    
    //Divide effects bufer to two buffers for modulation and delays
    unsigned int sub_bufs_sizes[2];
    sub_bufs_sizes[0] = mod_buf_size[0];
    sub_bufs_sizes[2] = EFFECTS_MEMORY_SIZE - mod_buf_size[0];

    if(EffectsInit(algorithms_buffer, sub_bufs_sizes, 2))
        return 1;
    else
        //Main loop
        while(1){}
}