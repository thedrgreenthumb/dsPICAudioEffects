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
    DAC1STATbits.ROEN = 1;   //Right Channel DAC Output Enable  d
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
