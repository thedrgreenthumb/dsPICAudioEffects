/**********************************************************************
* FileName:        Processing.c
* FileVersion      1.01
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

#include <libq.h>

#include "Effects.h"

#define INDICATION_PORT LATB

#define BUTTON0 PORTAbits.RA3 //Chain num
#define BUTTON1 PORTBbits.RB4 //Effect parameter
#define BUTTON2 PORTAbits.RA4 //Effect type
#define BUTTON3 PORTAbits.RA2//Bypass


#define DEBOUNCE_PROTECTION_VALUE 7000 //~0.3 s

#define CHAIN_POSITIONS 4
#define EFFECTS_IN_CHAIN_POSITION 4

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

//0          1         2          3          4          5          6          7          8          9
const char seven_sigm_digits[10]={0b00111111,0b00000110,0b01011011,0b01001111,0b01100110,0b01101101,0b01111101,0b00000111,0b01111111,0b01101111};

unsigned int effects_nums_in_chain_positions[CHAIN_POSITIONS];
//effects_in_chain_positions[0]=0; - bypass for chain position 0
//effects_in_chain_positions[0]=1; - hard clipping
//effects_in_chain_positions[0]=2; - soft clipping
//effects_in_chain_positions[0]=3; - compression
//effects_in_chain_positions[1]=0; - bypass for chain position 1
//effects_in_chain_positions[1]=1; - LP filter
//effects_in_chain_positions[1]=2; - BP filter
//effects_in_chain_positions[1]=3; - HP filter
//effects_in_chain_positions[2]=0; - bypass for chain position 2
//effects_in_chain_positions[2]=1; - chorus
//effects_in_chain_positions[2]=2; - flange
//effects_in_chain_positions[2]=3; - tremolo
//effects_in_chain_positions[3]=0; - bypass for chain position 3
//effects_in_chain_positions[3]=1; - delay
//effects_in_chain_positions[3]=2; - echo
//effects_in_chain_positions[3]=3; - reverb

unsigned int effects_parameter_vals_in_chain_positions[EFFECTS_IN_CHAIN_POSITION];
unsigned int current_chain_position;
unsigned int current_effect_num;
unsigned int current_parameter_val;
int is_bypass = 0;
int is_parameter_displays;
unsigned int debounce_counter;

static int is_button_pressed(int button_num);
static int is_debounce_protection(void);
static void seven_sigm_indicate(char in);
static void indicate_effect_character(void);
static void reset_all(void);

void controls_processing(void)
{
    //Buttons polling
    if(is_button_pressed(0) && !is_debounce_protection() && !is_bypass)
    {
        current_chain_position++;
        current_effect_num = 0;
        current_parameter_val = 0;
        if(current_chain_position > CHAIN_POSITIONS - 1)
        {
            current_chain_position = 0;
            reset_all();
        }
        debounce_counter = 0;
    }

    if(is_button_pressed(1) && !is_debounce_protection() && !is_bypass)
    {
        current_effect_num++;
        current_parameter_val = 0;
        if(current_effect_num > EFFECTS_IN_CHAIN_POSITION - 1)
            current_effect_num = 0;
        debounce_counter = 0;
    }

    if(is_button_pressed(2) && !is_debounce_protection() && (current_effect_num != 0) && !is_bypass)
    {
        current_parameter_val++;
        if(current_parameter_val > MAX_PARAMETER_VAL)
            current_parameter_val = 0;
        is_parameter_displays = 1;
        debounce_counter = 0;
    }

    if(is_button_pressed(3) && !is_debounce_protection())
    {
        is_bypass ^= 1;
        debounce_counter = 0;
    }

    //Update 
    effects_nums_in_chain_positions[current_chain_position] = current_effect_num;
    effects_parameter_vals_in_chain_positions[current_chain_position] = current_parameter_val;

    //Indication
    if(is_bypass)
        seven_sigm_indicate(BYPASS_CHARACTER);
    else if(is_parameter_displays)
        seven_sigm_indicate(seven_sigm_digits[current_parameter_val]);
    else
        indicate_effect_character();

    debounce_counter++;
    if(debounce_counter > DEBOUNCE_PROTECTION_VALUE)
        is_parameter_displays = 0;
}

//Define four runner structures for every chain position
algoRunner_t runners[4];

void samples_processing(_Q15 input_sample, _Q15* out_sample_L, _Q15* out_sample_R)
{
    _Q15 sample = 0;
    sample = dc_blocker(input_sample);

    if(!is_bypass)
    {	
		//Swap to loop of runners
        for(unsigned int i = 0; i < 4; i++)
		{
			AlgosRunnerSetParam(runners[i], 0, effects_parameter_vals_in_chain_positions[i]);
			AlgoSampleProcess(runners[i], &input_sample, &sample);
		}
		/*
		switch(effects_nums_in_chain_positions[0])
        {
            case 0:
                asm volatile ("NOP");
                break;
            case 1:
                sample = hard_clipping(sample, effects_parameter_vals_in_chain_positions[0]);
                break;
            case 2:
                sample = soft_clipping(sample, effects_parameter_vals_in_chain_positions[0]);
                break;
            case 3:
                sample = compression(sample, effects_parameter_vals_in_chain_positions[0]);
                break;
        }

        switch(effects_nums_in_chain_positions[1])
        {
            case 0:
                asm volatile ("NOP");
                break;
            case 1:
                sample = lp_filter(sample, effects_parameter_vals_in_chain_positions[1]);
                break;
            case 2:
                sample = bp_filter(sample, effects_parameter_vals_in_chain_positions[1]);
                break;
            case 3:
                sample = hp_filter(sample, effects_parameter_vals_in_chain_positions[1]);
                break;
        }

        switch(effects_nums_in_chain_positions[2])
        {
            case 0:
                asm volatile ("NOP");
                break;
            case 1:
                sample = chorus(sample, effects_parameter_vals_in_chain_positions[2]);
                break;
            case 2:
                sample = flange(sample, effects_parameter_vals_in_chain_positions[2]);
                break;
            case 3:
                sample = tremolo(sample, effects_parameter_vals_in_chain_positions[2]);
                break;
        }

        switch(effects_nums_in_chain_positions[3])
        {
            case 0:
                asm volatile ("NOP");
                break;
            case 1:
                sample = delay(sample, effects_parameter_vals_in_chain_positions[3]);
                break;
            case 2:
                sample = echo(sample, effects_parameter_vals_in_chain_positions[3]);
                break;
            case 3:
                sample = reverb(sample, effects_parameter_vals_in_chain_positions[3]);
                break;
        }
		*/
        signal_fork(sample, out_sample_L, out_sample_R);
    }
    else
    {
        *out_sample_L = sample;
        *out_sample_R = sample;
    }
}

static int is_button_pressed(int button_num)
{
    int is_pressed = 0;

    switch(button_num)
    {
        case 0:
            if(BUTTON0 == 0)
                is_pressed = 1;
            break;
        case 1:
            if(BUTTON1 == 0)
                is_pressed = 1;
            break;
        case 2:
            if(BUTTON2 == 0)
                is_pressed = 1;
            break;
        case 3:
            if(BUTTON3 == 0)
                is_pressed = 1;
            break;
        default:
            is_pressed = 0;
    }

    return is_pressed;
}

static int is_debounce_protection(void)
{
    int is_protected = 0;

    if(debounce_counter < DEBOUNCE_PROTECTION_VALUE)
        is_protected = 1;
    else
        is_protected = 0;

    return is_protected;
}

static void seven_sigm_indicate(char in)
{
   int out;
   
   out =(in&0b0000001)<<4;//A
   out|=(in&0b0000010)<<2;//B
   out|=(in&0b0000100)>>2;//C
   out|=(in&0b0001000)>>2;//D
   out|=(in&0b0010000)>>2;//E
   out|=(in&0b0100000);   //F
   out|=(in&0b1000000);   //G
   
   INDICATION_PORT = 0xFFFF^(out<<5);
}

static void indicate_effect_character(void)
{
    switch (current_chain_position)
    {
        case 0:
            switch (current_effect_num)
            {
                case 0:
                    seven_sigm_indicate(seven_sigm_digits[0]);
                    break;
                case 1:
                    seven_sigm_indicate(H_CHARACTER);
                    break;
                case 2:
                    seven_sigm_indicate(S_CHARACTER);
                    break;
                case 3:
                    seven_sigm_indicate(C_CHARACTER);
                    break;
            }
            break;
        case 1:
            switch (current_effect_num)
            {
                case 0:
                    seven_sigm_indicate(seven_sigm_digits[1]);
                    break;
                case 1:
                    seven_sigm_indicate(L_CHARACTER);
                    break;
                case 2:
                    seven_sigm_indicate(b_CHARACTER);
                    break;
                case 3:
                    seven_sigm_indicate(H_CHARACTER);
                    break;
            }
            break;
        case 2:
            switch (current_effect_num)
            {
                case 0:
                    seven_sigm_indicate(seven_sigm_digits[2]);
                    break;
                case 1:
                    seven_sigm_indicate(C_CHARACTER);
                    break;
                case 2:
                    seven_sigm_indicate(F_CHARACTER);
                    break;
                case 3:
                    seven_sigm_indicate(t_CHARACTER);
                    break;
            }
            break;
        case 3:
            switch (current_effect_num)
            {
                case 0:
                    seven_sigm_indicate(seven_sigm_digits[3]);
                    break;
                case 1:
                    seven_sigm_indicate(d_CHARACTER);
                    break;
                case 2:
                    seven_sigm_indicate(E_CHARACTER);
                    break;
                case 3:
                    seven_sigm_indicate(r_CHARACTER);
                    break;
            }
            break;
    }
}

static void reset_all(void)
{
    int i = 0;
    for(i = 0; i < CHAIN_POSITIONS; i++)
        effects_nums_in_chain_positions[i] = 0;

    for(i = 0; i < EFFECTS_IN_CHAIN_POSITION; i++)
        effects_parameter_vals_in_chain_positions[i] = 0;

    current_effect_num = 0;
    current_parameter_val = 0;
}
