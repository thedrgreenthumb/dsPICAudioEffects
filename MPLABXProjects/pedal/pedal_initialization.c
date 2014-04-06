/**********************************************************************
* FileName:        Initialization.c
* FileVersion      1.01
* Dependencies:    Initialization.h
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

#include <p33FJ128GP802.h>

#include "../runner/runner.h"
#include "../effects.h"
#include "pedal_processing.h"

void adc_init(void)
{
    //Connecting clock sourse
    AD1CON1bits.FORM   = 0b11;		// Data Output Format: Signed Fraction (Q15 format)
    AD1CON1bits.SSRC   = 0b111;         // Internal counter
    AD1CON1bits.ASAM   = 1;		// ADC Sample Control: Sampling begins immediately after conversion
    AD1CON1bits.AD12B  = 1;		// 12-bit ADC operation
    AD1CON1bits.ADDMABM = 0;            // DMA buffers are built in conversion order mode

    AD1CON2bits.CHPS  = 0;		// Converts CH0
    AD1CON2bits.SMPI    = 0;            // SMPI must be 0
    
    AD1CON3bits.ADRC = 0;               // ADC Clock is derived from Systems Clock
    AD1CON3bits.SAMC = 0;               // Auto Sample Time = 0 * TAD
    AD1CON3bits.ADCS = 0b00001110;      // ADC Conversion Clock min value
	
    //AD1CHS0: A/D Input Select Register
    AD1CHS0bits.CH0SA=0;		// MUXA +ve input selection (AN0) for CH0
    AD1CHS0bits.CH0NA=0;		// MUXA -ve input selection (Vref-) for CH0

    //AD1PCFGH/AD1PCFGL: Port Configuration Register
    AD1PCFGL=0xFFFF;
    AD1PCFGLbits.PCFG0 = 0;		// AN0 as Analog Input

    IFS0bits.AD1IF = 0;			// Clear the A/D interrupt flag bit
    IEC0bits.AD1IE = 0;			// Do Not Enable A/D interrupt 
    AD1CON1bits.ADON = 1;		// Turn on the A/D converter	
}

void audio_dac_init(void){

    //Oscillator configutration
    ACLKCONbits.SELACLK=0;     //Connect ACLK out to Fvco, 158,455 MHz
    ACLKCONbits.APSTSCLR=0b11; //Divide Fvco to 16 in oscillator, out freq=9903,5 kHz

    //Audio DAC configuration
    DAC1STATbits.LOEN = 1;   //Left Channel DAC Output Enabled
    DAC1STATbits.ROEN = 1;   //Right Channel DAC Output Enabled
    DAC1STATbits.LMVOEN=1;   //Midpoint enable for left channel
    DAC1STATbits.RMVOEN=1;   //Midpoint enable for right channel
    DAC1STATbits.LITYPE = 0; //Left Channel Interrupt if FIFO is not Full
    DAC1STATbits.RITYPE = 0; //Right Channel Interrupt if FIFO is not Full
    DAC1CONbits.AMPON = 0;   // Amplifier Disabled During Sleep and Idle Modes
    DAC1CONbits.DACFDIV = 1; // Divide Clock by 2 (Assumes Clock is 9903,5 kHz)
    DAC1CONbits.FORM = 1;    // Data Format is Signed int
    DAC1DFLT = 0x0000;       // Default value set to Midpoint when FORM = 0
    IFS4bits.DAC1RIF = 0;    // Clear Right Channel Interrupt Flag
    IFS4bits.DAC1LIF = 0;    // Clear Left Channel Interrupt Flag
    IEC4bits.DAC1LIE = 1;    // Left Channel Interrupt Enabled
    IEC4bits.DAC1RIE = 1;    // Right Channel Interrupt Enabled
    DAC1CONbits.DACEN = 1;   // DAC1 Module Enabled
}

void pins_init(void)
{
    //Buttons
    TRISAbits.TRISA2 = 1;
    LATAbits.LATA2=1;
    TRISAbits.TRISA3 = 1;
    LATAbits.LATA3=1;
    TRISAbits.TRISA4 = 1;
    LATAbits.LATA4=1;
    TRISBbits.TRISB4 = 1;
    LATBbits.LATB4=1;

    //7-sigment indicator
    TRISBbits.TRISB5 = 0;  //Segment G
    TRISBbits.TRISB6 = 0;  //Segment B
    TRISBbits.TRISB7 = 0;  //Segment F
    TRISBbits.TRISB8 = 0;  //Segment A
    TRISBbits.TRISB9 = 0;  //Segment E
    TRISBbits.TRISB10 = 0; //Segment D
    TRISBbits.TRISB11 = 0; //Segment C
}

//Here declared runners and effects structures
runner_t runners[CHAIN_POSITIONS];
bypass bp0;
hard_clipping hc;
soft_clipping sc;
compression comp;

bypass bp1;
lp_filter lp;
bp_filter bp;
hp_filter hp;

bypass bp2;
chorus chor;
flange fl;
tremolo tr;

bypass bp3;
delay del;
echo ech;
reverb rev;

int runners_init(_Q15* algorithms_buffer, unsigned int* sub_bufs_sizes, unsigned int number_of_gaps)
{
    error_t err = ERROR_OK;

    //Runner 0
    err = bypass_init(&bp0, algorithms_buffer);
    err = runner_add_effect(&runners[0], &bp0, bypass_set_params, bypass_process);

    err = hard_clipping_init(&hc, algorithms_buffer);
    err = runner_add_effect(&runners[0], &hc, hard_clipping_set_params, hard_clipping_process);

    err = soft_clipping_init(&sc, algorithms_buffer);
    err = runner_add_effect(&runners[0], &sc, soft_clipping_set_params, soft_clipping_process);

    err = compression_init(&comp, algorithms_buffer);
    err = runner_add_effect(&runners[0], &comp, compression_set_params, compression_process);

    //Runner 1
    err = bypass_init(&bp1, algorithms_buffer);
    err = runner_add_effect(&runners[1], &bp1, bypass_set_params, bypass_process);

    err = lp_filter_init(&lp, algorithms_buffer);
    err = runner_add_effect(&runners[1], &lp, lp_filter_set_params, lp_filter_process);

    err = bp_filter_init(&bp, algorithms_buffer);
    err = runner_add_effect(&runners[1], &bp, bp_filter_set_params, bp_filter_process);

    err = hp_filter_init(&hp, algorithms_buffer);
    err = runner_add_effect(&runners[1], &hp, hp_filter_set_params, hp_filter_process);

    //Runner 2
    err = bypass_init(&bp2, algorithms_buffer);
    err = runner_add_effect(&runners[2], &bp2, bypass_set_params, bypass_process);

    //Path buffer size thru first buf element
    algorithms_buffer[0] = sub_bufs_sizes[0];

    err = chorus_init(&chor, algorithms_buffer);
    err = runner_add_effect(&runners[2], &chor, chorus_set_params, chorus_process);

    err = flange_init(&fl, algorithms_buffer);
    err = runner_add_effect(&runners[2], &fl, flange_set_params, flange_process);

    err = tremolo_init(&tr, algorithms_buffer);
    err = runner_add_effect(&runners[2], &tr, tremolo_set_params, tremolo_process);

    //Runner 3
    err = bypass_init(&bp3, algorithms_buffer);
    err = runner_add_effect(&runners[3], &bp3, bypass_set_params, bypass_process);

    //Apply buffer gap for delay effects and path buffer size thru first buf element
    algorithms_buffer[sub_bufs_sizes[0]] = sub_bufs_sizes[1];

    err = delay_init(&del, &algorithms_buffer[sub_bufs_sizes[0]]);
    err = runner_add_effect(&runners[3], &del, delay_set_params, delay_process);

    err = echo_init(&ech, &algorithms_buffer[sub_bufs_sizes[0]]);
    err = runner_add_effect(&runners[3], &ech, echo_set_params, echo_process);

    err = reverb_init(&rev, &algorithms_buffer[sub_bufs_sizes[0]]);
    err = runner_add_effect(&runners[3], &rev, reverb_set_params, reverb_process);

    return err;
}