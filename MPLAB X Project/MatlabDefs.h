//Hard clipping
const _Q16 hard_clipping_dat[16]=
{
    #include "precomputes\hard_clipping.dat"
};
//Soft clipping
const _Q16 soft_clipping_dat[16]=
{
    #include "precomputes\soft_clipping.dat"
};
//Compression
const _Q16 compression_dat[11]=
{
    #include "precomputes\compression.dat"
};
#define COMPRESSION_TAV compression_dat[0]

//LP filter
const _Q16 lp_filter_dat[61]=
{
    #include "precomputes\lpfilter.dat"
};
#define LP_FILTER_COEFS_SETS_NUM lp_filter_dat[60]
//BP filter
const _Q16 bp_filter_dat[62]=
{
    #include "precomputes\bpfilter.dat"
};
#define BP_FILTER_INPUT_COEF bp_filter_dat[60]
#define BP_FILTER_COEFS_SETS_NUM bp_filter_dat[61]
//HP filter
const _Q16 hp_filter_dat[61]=
{
    #include "precomputes\hpfilter.dat"
};
#define HP_FILTER_COEFS_SETS_NUM hp_filter_dat[60]

_Q15 mod_effects_buf[
    #include "precomputes/mod_effects_buf.dat"
    ];
//chorus
const int chorus_dat[6010]=  //0..2999 - integer part, 3000..5999 - fractional part
{
    #include "precomputes\chorus.dat"
};
#define CHORUS_BUF_LEN chorus_dat[6000]
#define CHORUS_TAP_LENS(x) chorus_dat[(x)+6001]
#define CHORUS_INPUT_COEFS(x) chorus_dat[(x)+6003]
#define CHORUS_FEEDBACK_COEFS(x) chorus_dat[(x)+6005]
#define CHORUS_FEEDFORWARD_COEFS(x) chorus_dat[(x)+6007]
#define CHORUS_WAVE_TABLE_LEN chorus_dat[6009]
//flange
const int flange_dat[6005]=
{
    #include "precomputes\flange.dat"
};
#define FLANGE_WAVE_TABLE_LEN CHORUS_WAVE_TABLE_LEN
#define FLANGE_BUF_LEN flange_dat[6000]
#define FLANGE_TAP_LEN flange_dat[6001]
#define FLANGE_INPUT_COEF flange_dat[6002]
#define FLANGE_FEEDFORWARD_COEF flange_dat[6003]
#define FLANGE_FEEDBACK_COEF flange_dat[6004]

_Q15 delay_effects_buf[
    #include "precomputes\delay_effects_buf.dat"
    ];
//delay
const int delay_dat[3] =
{
  #include "precomputes/delay.dat"
};
#define DELAY_FF_COEF delay_dat[0]
#define DELAY_TAP_COEF delay_dat[1]
#define DELAY_BUF_LEN delay_dat[2]
//echo
const int echo_dat[4] =
{
  #include "precomputes/echo.dat"
};
#define ECHO_FF_COEF echo_dat[0]
#define ECHO_FB_COEF echo_dat[1]
#define ECHO_TAP_COEF echo_dat[2]
#define ECHO_BUF_LEN echo_dat[3]
//reverb
const int reverb_dat[14] =
{
  #include "precomputes/reverb.dat"
};
#define REVERB_FF_COEF reverb_dat[0]
#define REVERB_FB_COEF reverb_dat[1]
#define REVERB_TAP_COEFS(x) reverb_dat[(x) + 2] // see MATLAB Precomputes/precomputes.m
#define REVERB_TAP_LENS(x) reverb_dat[(x) + 6]
#define REVERB_APF_COEFS(x) reverb_dat[(x) + 9]
#define REVERB_BUF_LEN reverb_dat[11]
#define REVERB_APF_LENS(x) reverb_dat[(x) + 12]

const _Q15 int_to_Q15[]={0,3276,6553,9830,13106,16383,19660,22936,26213,29490};
