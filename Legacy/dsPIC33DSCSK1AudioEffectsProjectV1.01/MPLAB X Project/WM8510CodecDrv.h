/**********************************************************************
* FileName:        WM8510CodecDrv.h
* FileVersion      1.01
* Dependencies:
* Processor:       dsPIC33FJ256GP506
* Compiler:        C30 v3.21
*
* Design in:       SAL
* Design by:
* Feedback:
*
* Project:         dsPIC DSC Starter Kit 1 audio effects project.
* ProjectVersion:  1.01
**********************************************************************/
#include <p33FJ256GP506.h>

#define WM8510DRV_DCI_INTERRUPT_PRIORITY                        4
#define WM8510DRV_I2CBAUD					272
#define WM8510DRV_I2C_ADDR					0b00110100
#define WM8510DRV_DCICON1VAL                                    0x0700
#define WM8510DRV_DCICON2VAL                                    0x000F
#define WM8510DRV_TSCONVAL					0x1
#define WM8510DRV_RSCONVAL					0x1


#define	    WM8510DRV_I2C_TRISSCL _TRISG2
#define     WM8510DRV_I2C_TRISSDA _TRISG3


typedef union uCommandValueWord
{
	int wordValue;
	unsigned char bytes[2];
}CommandValueWord;	

/* WM8510 Codec Register Defines	*/
#define WM8510_SOFTWARE_RESET	0
#define WM8510_POWER_MGMT1	1
#define WM8510_POWER_MGMT2	2
#define WM8510_POWER_MGMT3	3
#define WM8510_AUDIO_INTERFACE	4
#define WM8510_COMPANDING_CTRL	5
#define WM8510_CLOCKGEN_CTRL	6
#define WM8510_ADDITIONAL_CTRL	7
#define WM8510_GPIO_STUFF		8
#define WM8510_DAC_CTRL			10
#define WM8510_DAC_VOLUME		11
#define WM8510_ADC_CONTROL		14
#define WM8510_ADC_DIGITAL_VOL	15
#define WM8510_DAC_LIMITER1		24
#define WM8510_DAC_LIMITER2		25
#define WM8510_NOTCH_FILTER1	27
#define WM8510_NOTCH_FILTER2	28
#define WM8510_NOTCH_FILTER3	29
#define WM8510_NOTCH_FILTER4	30
#define WM8510_ALC_CONTROL1	32
#define WM8510_ALC_CONTROL2	33
#define WM8510_ALC_CONTROL3	34
#define WM8510_NOISE_GATE		35
#define WM8510_PLL_N			36
#define WM8510_PLL_K1			37
#define WM8510_PLL_K2			38
#define WM8510_PLL_K3			39
#define WM8510_ATTEN_CTRL		40
#define WM8510_INPUT_CTRL		44
#define WM8510_INP_PGA_CTRL		45
#define WM8510_ADC_BOOST_CTRL	47
#define WM8510_OUTPUT_CTRL		49
#define WM8510_SPK_MIXER_CTRL	50
#define WM8510_SPK_VOLUME_CTRL	54
#define WM8510_MONO_MIXER_CTRL	56


void 	WM8510Init		(void);
void 	WM8510Start		(void);
int 	WM8510IOCtl		(int command, void * value);

void WM8510SampleRate8KConfig(void);
void WM8510SampleRate16KConfig(void);
void WM8510SampleRate40KConfig(void);
void WM8510SampleRate48KConfig(void);
