/**********************************************************************
* FileName:        main.c
* FileVersion      1.01
* Dependencies:
* Processor:       dsPIC33FJ256GP506
* Compiler:        C30 v3.21
*
* License:         MIT
*
* Design in:       SAL
* Design by:
* Feedback:
*
* Project:         dsPIC DSC Starter Kit 1 Audio Effects Project.
* ProjectVersion:  0.01
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

//Runner and effects
#include "../runner/runner.h"
#include "../runner/runner_errors.h"
#include "../foundation.h"
#include "../effects.h"

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

runner_t runner;
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
#define DELAYS_MEMORY_SIZE (7000)
_Q15 delays_buffer[DELAYS_MEMORY_SIZE];

bypass bp;
hard_clipping hc;
bp_filter bpf;
flange fl;
tremolo tr;
delay del;
echo ech;
reverb rev;

int effects_init(runner_t* runner, _Q15* algorithms_buffer, unsigned int sub_bufs_size)
{
    error_t err = ERROR_OK;

    err += bypass_init(&bp, NULL);
    err += runner_add_effect(runner, &bp, bypass_set_params, bypass_process);

    err += hard_clipping_init(&hc, NULL);
    err += runner_add_effect(runner, &hc, hard_clipping_set_params, hard_clipping_process);

    err += bp_filter_init(&bpf, NULL);
    err += runner_add_effect(runner, &bpf, bp_filter_set_params, bp_filter_process);

    err += flange_init(&fl, algorithms_buffer);
    err += runner_add_effect(runner, &fl, flange_set_params, flange_process);

    err += tremolo_init(&tr, NULL);
    err += runner_add_effect(runner, &tr, tremolo_set_params, tremolo_process);

    //Pasbuffer size thru first element
    algorithms_buffer[0] = sub_bufs_size;

    err += delay_init(&del, algorithms_buffer);
    err += runner_add_effect(runner, &del, delay_set_params, delay_process);

    err += echo_init(&ech, algorithms_buffer);
    err += runner_add_effect(runner, &ech, echo_set_params, echo_process);

    err += reverb_init(&rev, algorithms_buffer);
    err += runner_add_effect(runner, &rev, reverb_set_params, reverb_process);

    algorithms_buffer[0] = 0;

    return err;
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

    __builtin_write_OSCCONH(0x01);	/*	Initiate Clock Switch to FRC with PLL*/
    __builtin_write_OSCCONL(0x01);
    while (OSCCONbits.COSC != 0b01);	/*	Wait for Clock switch to occur	*/
    while(!OSCCONbits.LOCK);

    if(effects_init(&runner, delays_buffer, DELAYS_MEMORY_SIZE))
    {
        int disp = 7;
        //Indicate initialization error
        LATC = (~disp) << 13;
        return 1;
    }
    
    //Ext. int 1
    IPC5=0b0000000000000010;
    //Ext. int 2 
    IPC7=0b0000001100100000;
 
    //Output amplifier control pins initialization
    LM4811PinsInit();
    LM4811SetVolDOWN(32); //Set Volume of LM4811 to min value
    InitLEDsAndBUTs();

    //Codec initialization
    WM8510Init();
    WM8510Start();
    WM8510SampleRate16KConfig();

    while(1){}
}
