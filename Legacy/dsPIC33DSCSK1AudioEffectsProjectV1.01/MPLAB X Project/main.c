/**********************************************************************
* FileName:        main.ñ
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
#include <p33FJ256GP506.h>
//Math
#include <math.h>
#include <libq.h>
//Board drivers
#include "Initializtion.h"
#include "WM8510CodecDrv.h"
//Algorithms 
#include "Algorithms.h"

//Configuration bits
_FGS(GWRP_OFF & GCP_OFF);
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE);
_FWDT(FWDTEN_OFF);

//Processing control variables
//Algorithm number
unsigned int algNum = 0;
//Algorithm control value
unsigned int algVal = 0;
//This vector require for converting integer values 0..10 to fractional values 0..0.99
const _Q15 intToQ15[]={0,3276,6553,9830,13106,16383,19660,22936,26213,29490,32767};

//Button S1
void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void)
{
         algVal = 0;
            algNum++;
            if(algNum > 7)
                algNum = 0;

             //Algorithm number indication to LEDs
            switch (algNum)
            {
            case 0:
                LATC = 0b1110000000000000;
                break;
            case 1:
                LATC = 0b1010000000000000;
                break;
            case 2:
                LATC = 0b1100000000000000;
                break;
            case 3:
                LATC = 0b1000000000000000;
                break;
            case 4:
                LATC = 0b0110000000000000;
                break;
            case 5:
                LATC = 0b0010000000000000;
                break;
            case 6:
                LATC = 0b0100000000000000;
                break;
            case 7:
                LATC = 0b0000000000000000;
                break;
            }

        IFS1bits.INT1IF = 0;    //Clear the INT1 interrupt flag or else
                                //the CPU will keep vectoring back to the ISR
}

//Button S2
void __attribute__((interrupt, no_auto_psv)) _INT2Interrupt(void)
{
         //Button S2
        if (PORTDbits.RD9 == 0)
        {
            algVal++;
            if(algVal > 9)
                algVal = 0;
        }

        IFS1bits.INT2IF = 0;    //Clear the INT2 interrupt flag or else
                                //the CPU will keep vectoring back to the ISR
}

//Processing variabls and constants
//Sample Var
_Q15 sample = 0;
//One sample buffer for filter (AlgNum = 2)
_Q15 fltBuf = 0;
//Sin counter (AlgNum = 3)
_Q15 sinWaveCounter = 0;
//Short delay line (FIFO)(AlgNum = 4,AlgNum = 5)
_Q15 delaySdlBuf0[50]={0};
//Wave tables
const int waveTablesInt[1000]=
{
	#include "WaveTables\sin_int_part_wave_table_43.dat"
};
const _Q15 waveTablesFrac[1000]=
{
	#include "WaveTables\sin_frac_part_wave_table_43.dat"
};
//Wave tables counters
unsigned int firstWtCounter = 0;
unsigned int secondWtCounter = 0;
//Main circular bufer (AlgNum = 6, AlgNum = 7)
_Q15 delayBuf0[7000]={0};//All RAM 8000 16-bit words
//Tap counter
unsigned int firstTapCount=0;
//Counters for AlgNum = 7
unsigned int revCounter0 = 0;
unsigned int revCounter1 = 0;
unsigned int revCounter2 = 0;
unsigned int revCounter3 = 0;

//DCI interrupt (sample by sample processing)
void __attribute__((__interrupt__,no_auto_psv)) _DCIInterrupt(void)
{
    //Input sample comes in Q15
    sample = RXBUF0;
    //Applaying algorithms to sample variable
    if (algNum == 1)
       sample = HardClipping(sample,24576 - Q15mpy(intToQ15[algVal], 24576));

    if (algNum == 2)
       sample = OnePoleFilter(sample,32768 - intToQ15[algVal], &fltBuf);

    if (algNum == 3)//Tremolo
    {
        sample = Q15mpy(sample,_Q15sinPI(sinWaveCounter));
        //Counter updating for tremolo
        sinWaveCounter += 2*(algVal+1);
        if (sinWaveCounter >= 32767)
            sinWaveCounter =- 32767;
    }
       
    if (algNum == 4 || algNum == 5)
    {
        if (algNum == 4)//Vibrato
			sample=
              LInterpDelLine(sample, &delaySdlBuf0[0],50, waveTablesInt[secondWtCounter], waveTablesFrac[secondWtCounter]);
        else//Flange
           sample = Q15mpy(sample,16384)
        +Q15mpy(LInterpDelLine(sample, &delaySdlBuf0[0],50, waveTablesInt[secondWtCounter], waveTablesFrac[secondWtCounter]),16384)
        +Q15mpy(delaySdlBuf0[21],8192);
        //Wave tables counters update for Flange and Vibrato
        firstWtCounter++;
        if (firstWtCounter >= (2+(9-algVal))/2)
        {
            secondWtCounter++;
            if (secondWtCounter >= 1000)
                secondWtCounter = 0;
            firstWtCounter = 0;
        }
    }

    if (algNum == 6)//Single tap delay
        sample=Q15mpy(16384,sample)+
            Q15mpy(16384,DelayLine(sample, &delayBuf0[0], &firstTapCount, 699*algVal + 1));

    if (algNum == 7)//Schoeder's 1 reverberator
    {
        int p0 = 0;
        int p1 = 0;
        int p2 = 0;
        p0 =AllPassWithDelayLine(sample, &delayBuf0[0], &revCounter0, 959, Q15mpy(intToQ15[algVal],intToQ15[8]));
        p1 = AllPassWithDelayLine(p0, &delayBuf0[959], &revCounter1, 731, Q15mpy(intToQ15[algVal],intToQ15[7]));
        p2 = AllPassWithDelayLine(p1, &delayBuf0[959 + 731], &revCounter2, 687, Q15mpy(intToQ15[algVal],intToQ15[6]));
        sample = AllPassWithDelayLine(p2, &delayBuf0[959 + 731 + 687], &revCounter3, 555, Q15mpy(intToQ15[algVal],intToQ15[5]));
    }
	
    TXBUF0 = sample;
    //Output in Q15

    // Close interrupt flag
    IFS3bits.DCIIF = 0;
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
    
    //Main loop
    while(1){}
}
