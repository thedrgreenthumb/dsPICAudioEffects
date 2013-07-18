const _Q15 int_to_Q15[]={0,3276,6553,9830,13106,16383,19660,22936,26213,29490};

//chorus
const int chorus_wave_table[6000]=  //0..2999 - integer part, 3000..5999 - fractional part
{
    #include "precomputes\chorus.dat"
};

const int flange_wave_table[6000]=
{
    #include "precomputes\flange.dat"
};

//delay
const int delay_dat[2] =
{
  #include "precomputes/delay.dat"
};
#define DELAY_TAP_COEF delay_dat[0]
#define DELAY_BUF_LEN delay_dat[1]
//echo
const int echo_dat[2] =
{
  #include "precomputes/echo.dat"
};
#define ECHO_TAP_COEF echo_dat[0]
#define ECHO_BUF_LEN echo_dat[1]
//reverb
const int reverb_dat[12] =
{
  #include "precomputes/reverb.dat"
};
#define REVERB_TAP_COEFS(x) reverb_dat[(x) + 0] // see MATLAB Precomputes/precomputes.m
#define REVERB_TAP_LENS(x) reverb_dat[(x) + 4]
#define REVERB_APF_COEFS(x) reverb_dat[(x) + 7]
#define REVERB_BUF_LEN reverb_dat[9]
#define REVERB_APF_LENS(x) reverb_dat[(x) + 10]