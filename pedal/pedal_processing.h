/**********************************************************************
* FileName:        Processing.h
* FileVersion      1.01
* Processor:       dsPIC33FJ128GP802
* Compiler:        C30 v3.21
*
* Design in:       SAL
* Design by:
* Feedback:
*
* Project:         dsPIC33 Audio Effects Pedal project.
* Project version: 0.01
**********************************************************************/

#include <libq.h>

#define CHAIN_POSITIONS (4)
#define EFFECTS_IN_CHAIN_POSITION (4)

#define INDICATION_PORT LATB

#define BUTTON0 PORTAbits.RA3 //Chain num
#define BUTTON1 PORTBbits.RB4 //Effect parameter
#define BUTTON2 PORTAbits.RA4 //Effect type
#define BUTTON3 PORTAbits.RA2 //Bypass

#define DEBOUNCE_PROTECTION_VALUE (7000) //~0.3 s

//Some characters difinitions for indication
#define b_CHARACTER 0b01111100
#define C_CHARACTER 0b00111001
#define d_CHARACTER 0b01011110
#define E_CHARACTER 0b01111001
#define F_CHARACTER 0b01110001
#define H_CHARACTER 0b01110110
#define L_CHARACTER 0b00111000
#define r_CHARACTER 0b01010000
#define S_CHARACTER 0b01101101
#define t_CHARACTER 0b01111000
#define BYPASS_CHARACTER 0b01000000

int runners_init(_Q15* mod_buf, unsigned int mod_buf_sz,
        _Q15* delays_buf, unsigned int delays_buf_sz);

void controls_processing(void);
void samples_processing(_Q15 input_sample, _Q15* out_sample_L, _Q15* out_sample_R);

void seven_sigm_indicate(char in);
void seven_sigm_indicate_num(unsigned int i);
int is_button_pressed(int button_num);

