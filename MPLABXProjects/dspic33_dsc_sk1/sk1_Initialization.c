/**********************************************************************
* FileName:        sk1_Initialization.c
* FileVersion      0.01
* Dependencies:    Initialization.h
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
#include "./sk1_Initializtion.h"

//Runner and effects
#include "../runner/runner.h"
#include "../runner/runner_errors.h"
#include "../foundation.h"
#include "../effects.h"

void InitLEDsAndBUTs(void)
{
    //Buttons
    INTCON2 = 0x6   ;       /*Setup INT1, INT2,  pins to interupt */
    IFS1bits.INT1IF = 0;    /*Reset INT1 interrupt flag */
    IEC1bits.INT1IE = 1;    /*Enable INT1 Interrupt Service Routine */
    IFS1bits.INT2IF = 0;    /*Reset INT0 interrupt flag */
    IEC1bits.INT2IE = 1;    /*Enable INT0 Interrupt Service Routine */

    //Led's configuration
    //Amber LED
    TRISCbits.TRISC13 = 0;
    PORTCbits.RC13 = 1;
    //Green LED 
    TRISCbits.TRISC14 = 0;
    PORTCbits.RC14 = 1;
    //Red LED 
    TRISCbits.TRISC15 = 0;
    PORTCbits.RC15 = 1;
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

//LM4811 pins configuration
void LM4811PinsInit(void){

 TRISGbits.TRISG0 = 0;
 PORTGbits.RG0 = 0;

 TRISGbits.TRISG1 = 0;
 PORTGbits.RG1 = 0;


}

//LM4811 Volume setup (min - 0, max - 32)
void LM4811SetVolUP(int volVal)
{  
    int i = 0;
    for (i = 0; i < volVal; i++ )
    {
        Delay(500);
        //+Volume
        //U/D
        LATGbits.LATG0 = 1;
        //CLK
        LATGbits.LATG1 = 1;
        Delay(500);
        LATGbits.LATG1 = 0;
    }
}

//LM4811 Volume setup (min - 0, max - 32)
void LM4811SetVolDOWN(int volVal)
{  
    int i = 0;
    for (i = 0; i < volVal; i++ )
    {
        Delay(500);
        //-Volume
        //U/D
        LATGbits.LATG0 = 0;
        //CLK
        LATGbits.LATG1 = 1;
        Delay(500);
            LATGbits.LATG1 = 0;
    }
}

runner_t runner;

bypass bp;
hard_clipping hc;
bp_filter bpf;
flange fl;
tremolo tr;
delay del;
echo ech;
reverb rev;

int effects_init(_Q15* algorithms_buffer, unsigned int sub_bufs_size)
{
    error_t err = ERROR_OK;

    err += bypass_init(&bp, NULL);
    err += runner_add_effect(&runner, &bp, bypass_set_params, bypass_process);

    err += hard_clipping_init(&hc, NULL);
    err += runner_add_effect(&runner, &hc, hard_clipping_set_params, hard_clipping_process);

    err += bp_filter_init(&bpf, NULL);
    err += runner_add_effect(&runner, &bpf, bp_filter_set_params, bp_filter_process);

    err += flange_init(&fl, algorithms_buffer);
    err += runner_add_effect(&runner, &fl, flange_set_params, flange_process);

    err += tremolo_init(&tr, NULL);
    err += runner_add_effect(&runner, &tr, tremolo_set_params, tremolo_process);

    //Pasbuffer size thru first element
    algorithms_buffer[0] = sub_bufs_size;

    err += delay_init(&del, algorithms_buffer);
    err += runner_add_effect(&runner, &del, delay_set_params, delay_process);

    err += echo_init(&ech, algorithms_buffer);
    err += runner_add_effect(&runner, &ech, echo_set_params, echo_process);

    err += reverb_init(&rev, algorithms_buffer);
    err += runner_add_effect(&runner, &rev, reverb_set_params, reverb_process);

     algorithms_buffer[0] = 0;

    return err;
}
