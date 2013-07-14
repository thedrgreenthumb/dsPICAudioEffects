/**********************************************************************
* FileName:        Effects.c
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

#include <libq.h>

#include "FoundAlgorithms.h"

//LP filter coefs
const float lp_filter_coefs[60]=
{
    #include "precomputes\lpfilter.dat"
};

//BP filter coefs
const float bp_filter_coefs[60]=
{
    #include "precomputes\bpfilter.dat"
};

//HP filter coefs
const float hp_filter_coefs[60]=
{
    #include "precomputes\hpfilter.dat"
};

const int chorus_wave_table[6000]=  //0..2999 - integer part, 3000..5999 - fractional part
{
    #include "precomputes\chorus.dat"
};
const int flange_wave_table[6000]=
{
    #include "precomputes\flange.dat"
};


const _Q15 int_to_Q15[]={0,3276,6553,9830,13106,16383,19660,22936,26213,29490};

static _Q15 delay_effects_buf[7000];
static _Q15 mod_effects_buf[100];

_Q15 dc_blocker_coef;
_Q15 dc_blocker_buf[2];
_Q15 dc_blocker(_Q15 sample)
{
    _Q15 bypass_sample = 0;
    bypass_sample = sample;
    sample = sample - dc_blocker_buf[0]+Q15mpy(dc_blocker_coef,dc_blocker_buf[1]);
    dc_blocker_buf[0] = bypass_sample;
    dc_blocker_buf[1] = sample;

    return sample;
}

_Q15 hard_clipping(_Q15 sample, unsigned int parameter_val)
{
    return sample;
}

_Q15 soft_clipping(_Q15 sample, unsigned int parameter_val)
{
    return sample;
}

_Q15 compression(_Q15 sample, unsigned int parameter_val)
{
    return sample;
}

_Q16 lp_filter_num[30];
_Q16 lp_filter_denum[30];
_Q16 lp_filter_buf[4];
_Q15 lp_filter(_Q15 sample, unsigned int parameter_val)
{
    _Q15 out = 0;

    _Q16 b[3];
    _Q16 a[3];

    b[0] = lp_filter_num[parameter_val];
    b[1] = lp_filter_num[parameter_val + 10];
    b[2] = lp_filter_num[parameter_val + 20];

    a[0] = lp_filter_denum[parameter_val];
    a[1] = lp_filter_denum[parameter_val + 10];
    a[2] = lp_filter_denum[parameter_val + 20];
    
    out = Q16toQ15(DF2SOStructure(Q15toQ16(sample), b, a, lp_filter_buf, &lp_filter_buf[2]));

    return out;
}

_Q16 bp_filter_num[30];
_Q16 bp_filter_denum[30];
_Q16 bp_filter_buf[4];
_Q15 bp_filter(_Q15 sample, unsigned int parameter_val)
{
    _Q15 out = 0;

    _Q16 b[3];
    _Q16 a[3];

    b[0] = bp_filter_num[parameter_val];
    b[1] = bp_filter_num[parameter_val + 10];
    b[2] = bp_filter_num[parameter_val + 20];

    a[0] = bp_filter_denum[parameter_val];
    a[1] = bp_filter_denum[parameter_val + 10];
    a[2] = bp_filter_denum[parameter_val + 20];

    out = Q16toQ15(DF2SOStructure(Q16mpy(Q15toQ16(sample), _Q16ftoi(0.4)), b, a, bp_filter_buf, &bp_filter_buf[2]));

    return sample;
}

_Q16 hp_filter_num[30];
_Q16 hp_filter_denum[30];
_Q16 hp_filter_buf[4];
_Q15 hp_filter(_Q15 sample, unsigned int parameter_val)
{
    _Q15 out = 0;

    _Q16 b[3];
    _Q16 a[3];

    b[0] = hp_filter_num[parameter_val];
    b[1] = hp_filter_num[parameter_val + 10];
    b[2] = hp_filter_num[parameter_val + 20];

    a[0] = hp_filter_denum[parameter_val];
    a[1] = hp_filter_denum[parameter_val + 10];
    a[2] = hp_filter_denum[parameter_val + 20];

    out = Q16toQ15(DF2SOStructure(Q15toQ16(sample), b, a, hp_filter_buf, &hp_filter_buf[2]));

    return out;
}

unsigned int chorusCounters[4] = {0,0,1500,1500};
unsigned int chorus_counters[2];
_Q15 chorus_fb_point[2];
_Q15 chorus(_Q15 sample, unsigned int parameter_val)
{
    sample = Q15mpy(sample,16384);
    sample = Q15mpy(sample,27000)
        +li_delay_line(sample + chorus_fb_point[0], &mod_effects_buf[0], &chorus_counters[0], 300, chorus_wave_table[chorusCounters[1]], chorus_wave_table[chorusCounters[1]+3000]);
        +li_delay_line(sample + chorus_fb_point[1], &mod_effects_buf[50], &chorus_counters[1], 300, chorus_wave_table[chorusCounters[3]], chorus_wave_table[chorusCounters[3]]+3000);

    chorus_fb_point[0] = Q15mpy(16384, delay_line_tap(150, &mod_effects_buf[0], chorus_counters[0], 300));
    chorus_fb_point[1] = Q15mpy(16384, delay_line_tap(150, &mod_effects_buf[50], chorus_counters[1], 300));


        //Chorus counters update
        chorusCounters[0]++;
        if (chorusCounters[0] >= (1+2*(9-parameter_val)))
        {
            chorusCounters[1]++;
            if (chorusCounters[1] >= 3000)
                chorusCounters[1]=0;
            chorusCounters[0]=0;
        }
        chorusCounters[2]++;
        if (chorusCounters[2] >= (1+1*(9-parameter_val)))
        {
            chorusCounters[3]++;
            if (chorusCounters[3] >= 3000)
                chorusCounters[3] = 0;
            chorusCounters[2]=0;
        }

    return sample;
}

unsigned int flange_counter;
unsigned int flange_wt_counter0;
unsigned int flange_wt_counter1;
_Q15 flange_fb_point;
_Q15 flange(_Q15 sample, unsigned int parameter_val)
{
    sample = Q15mpy(sample + flange_fb_point, 16384) +
                Q15mpy(25000, li_delay_line(sample, &mod_effects_buf[0], &flange_counter,30, flange_wave_table[flange_wt_counter1], flange_wave_table[flange_wt_counter1 + 3000]));
    flange_fb_point = Q15mpy(16384, delay_line_tap(20, &mod_effects_buf[0], flange_counter, 15));

    flange_wt_counter0++;
    if (flange_wt_counter0 >= (1+(9-parameter_val)))
    {
        flange_wt_counter1++;
        if (flange_wt_counter1 >= 3000)
            flange_wt_counter1 = 0;
        flange_wt_counter0 = 0;
    }

    return sample;
}

_Q15 tremolo_counter;
_Q15 tremolo(_Q15 sample, unsigned int parameter_val)
{
    sample = Q15mpy(sample,Q15mpy(16384,_Q15sinPI(tremolo_counter)));

    tremolo_counter+=2*(parameter_val+1);
    if (tremolo_counter >= 32767)
        tremolo_counter = -32767;

    return sample;
}

_Q15 delay_coef;
unsigned int delay_counter;
_Q15 delay(_Q15 sample, unsigned int parameter_val)
{
    sample = sample + Q15mpy(delay_coef, delay_line(sample, delay_effects_buf, &delay_counter, 699*parameter_val + 1));

    return sample;
}

_Q15 echo_coef0;
_Q15 echo_fb_point;
unsigned int echo_counter;
_Q15 echo(_Q15 sample, unsigned int parameter_val)
{
    sample += Q15mpy(echo_coef0, delay_line(sample + Q15mpy(int_to_Q15[parameter_val],echo_fb_point), delay_effects_buf, &echo_counter, 7000));
    echo_fb_point = sample;

    return sample;
}

_Q15 reverb_coef[6];
_Q15 reverb_fb_p;
unsigned int reverb_counter[3];
_Q15 reverb(_Q15 sample, unsigned int parameter_val)
{
    sample += Q15mpy(reverb_coef[0], delay_line(sample + Q15mpy(Q15mpy(15000,int_to_Q15[parameter_val]),reverb_fb_p), delay_effects_buf, &reverb_counter[0], 6500));
    sample += Q15mpy(reverb_coef[1], delay_line_tap(1521, delay_effects_buf, reverb_counter[0], 6500));
    sample += Q15mpy(reverb_coef[2], delay_line_tap(2963, delay_effects_buf, reverb_counter[0], 6500));
    sample += Q15mpy(reverb_coef[3], delay_line_tap(4497, delay_effects_buf, reverb_counter[0], 6500));
    sample = all_pass(sample, reverb_coef[4], &delay_effects_buf[6500], &reverb_counter[1], 357);
    sample = all_pass(sample, reverb_coef[5], &delay_effects_buf[6500 + 357], &reverb_counter[2], 129);
    reverb_fb_p = sample;

    return sample;
}

void panoram_enhancer(_Q15 sample, _Q15* out_sample_L, _Q15* out_sample_R)
{
    *out_sample_L = sample;
    *out_sample_R = sample;
}

void effects_precomputes(void)
{
    //dc_blocker
    dc_blocker_coef = _Q15ftoi(0.98);


    //LP filter
    int i = 0;
    for(i = 0; i < 10; i++)
        lp_filter_denum[i] = _Q16ftoi(lp_filter_coefs[i]);//a0
    for(i = 0;  i < 10; i++)
        lp_filter_num[i] = _Q16ftoi(lp_filter_coefs[i + 10]);//b0
    for(i = 10; i < 20; i++ )
        lp_filter_denum[i] = _Q16ftoi(lp_filter_coefs[i + 10]);//a1
    for(i = 10; i < 20; i++ )
        lp_filter_num[i] = _Q16ftoi(lp_filter_coefs[i + 20]);//b1
    for(i = 20; i < 30; i++ )
        lp_filter_denum[i] = _Q16ftoi(lp_filter_coefs[i + 20]);//a2
    for(i = 20; i < 30; i++ )
       lp_filter_num[i] = _Q16ftoi(lp_filter_coefs[i + 30]);//b2

    //BP filter
    //int i = 0;
    for(i = 0; i < 10; i++)
        bp_filter_denum[i] = _Q16ftoi(bp_filter_coefs[i]);//a0
    for(i = 0;  i < 10; i++)
        bp_filter_num[i] = _Q16ftoi(bp_filter_coefs[i + 10]);//b0
    for(i = 10; i < 20; i++ )
        bp_filter_denum[i] = _Q16ftoi(bp_filter_coefs[i + 10]);//a1
    for(i = 10; i < 20; i++ )
        bp_filter_num[i] = _Q16ftoi(bp_filter_coefs[i + 20]);//b1
    for(i = 20; i < 30; i++ )
        bp_filter_denum[i] = _Q16ftoi(bp_filter_coefs[i + 20]);//a2
    for(i = 20; i < 30; i++ )
       lp_filter_num[i] = _Q16ftoi(bp_filter_coefs[i + 30]);//b2

    //HP filter
    //int i = 0;
    for(i = 0; i < 10; i++)
        hp_filter_denum[i] = _Q16ftoi(hp_filter_coefs[i]);//a0
    for(i = 0;  i < 10; i++)
        hp_filter_num[i] = _Q16ftoi(hp_filter_coefs[i + 10]);//b0
    for(i = 10; i < 20; i++ )
        hp_filter_denum[i] = _Q16ftoi(hp_filter_coefs[i + 10]);//a1
    for(i = 10; i < 20; i++ )
        hp_filter_num[i] = _Q16ftoi(hp_filter_coefs[i + 20]);//b1
    for(i = 20; i < 30; i++ )
        hp_filter_denum[i] = _Q16ftoi(hp_filter_coefs[i + 20]);//a2
    for(i = 20; i < 30; i++ )
       hp_filter_num[i] = _Q16ftoi(hp_filter_coefs[i + 30]);//b2

    //delay
    delay_coef = _Q15ftoi(0.5);
    //echo
    echo_coef0 = _Q15ftoi(0.5);
    //reverb
    reverb_coef[0] = _Q15ftoi(0.7);
    reverb_coef[1] = _Q15ftoi(0.5);
    reverb_coef[2] = _Q15ftoi(0.3);
    reverb_coef[3] = _Q15ftoi(0.22);
    reverb_coef[4] = _Q15ftoi(0.18);
    reverb_coef[5] = _Q15ftoi(0.12);
}



