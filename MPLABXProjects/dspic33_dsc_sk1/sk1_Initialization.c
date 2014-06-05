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


