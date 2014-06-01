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

#include "../effects.h"
#include "pedal_processing.h"

static int is_debounce_protection(void);
static void indicate_effect_character(void);
static void reset_all(void);
static char i_to_seven_dig(unsigned int);

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
        seven_sigm_indicate(i_to_seven_dig(current_parameter_val));
    else
        indicate_effect_character();

    debounce_counter++;
    if(debounce_counter > DEBOUNCE_PROTECTION_VALUE)
        is_parameter_displays = 0;
}

void samples_processing(_Q15 input_sample, _Q15* out_sample_L, _Q15* out_sample_R)
{
    _Q15 sample = 0;
    sample = dc_blocker(input_sample);

    if(!is_bypass)
    {
        //Runners loop
        unsigned int i = 0;
        for(i = 0; i < CHAIN_POSITIONS; i++)
        {
            runner_set_effect_num(&runners[i], effects_nums_in_chain_positions[i]);
            runner_set_param(&runners[i], 0, effects_parameter_vals_in_chain_positions[i]);
            runner_sbs_process(&runners[i], &sample, &sample);
        }
	
        /*
        runner_set_effect_num(&runners, 0);
        runner_set_param(&runners, 0, 1);
        runner_sbs_process(&runners, &sample, &sample);
        */
        
        *out_sample_L = sample;
        *out_sample_R = sample;
    }
    else
    {
        *out_sample_L = sample;
        *out_sample_R = sample;
    }
}

int is_button_pressed(int button_num)
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

void seven_sigm_indicate(char in)
{
   int out = 0;
   
   out =(in&0b0000001)<<4;//A
   out|=(in&0b0000010)<<2;//B
   out|=(in&0b0000100)>>2;//C
   out|=(in&0b0001000)>>2;//D
   out|=(in&0b0010000)>>2;//E
   out|=(in&0b0100000);   //F
   out|=(in&0b1000000);   //G
   
   INDICATION_PORT = 0xFFFF^(out<<5);
}

void seven_sigm_indicate_num(unsigned int i)
{
    seven_sigm_indicate(i_to_seven_dig(i));
}

static void indicate_effect_character(void)
{
    switch (current_chain_position)
    {
        case 0:
            switch (current_effect_num)
            {
                case 0:
                    seven_sigm_indicate(i_to_seven_dig(0));
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
                    seven_sigm_indicate(i_to_seven_dig(1));
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
                    seven_sigm_indicate(i_to_seven_dig(2));
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
                    seven_sigm_indicate(i_to_seven_dig(3));
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

static char i_to_seven_dig(unsigned int i)
{
    //0         1         2          3          4          5          6          7          8          9
    const char seven_sigm_digits[10]={0b00111111,0b00000110,0b01011011,0b01001111,0b01100110,0b01101101,0b01111101,0b00000111,0b01111111,0b01101111};

    return seven_sigm_digits[i];
}