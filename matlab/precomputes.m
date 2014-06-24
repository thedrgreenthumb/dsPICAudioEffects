%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FileName:        precomputes.m
% FileVersion      1.01
%
% MATLAB v:        8.0.0 (R2012b)
%
% Design by:       SAL
% Feedback:
%				  
% License:         MIT
%
% ADDITIONAL NOTES:
%                        dsPIC33 Pedal Project
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

Fs = 10000;

number_of_steps = 10;

len = 1000;
x(len) = 0;
x(1) = 1;
y(10,len) = 0;

b0(3) = 0;
a0(3) = 0;
b(3,10) = 0;
a(3,10) = 0;

fa = fanalizer(Fs);

%Precomputes for chain position = 0%%%

%Hard Clipping (hc)
hc_gain_coefs = [1;5;10;20;50;100;150;200;250;300];
for n=1:length(hc_gain_coefs) hc_gain_coefs(n) = toDspicQ16(hc_gain_coefs(n)); end;
hc_gain_coefs_sz =  size(hc_gain_coefs);
fsaver.savePlaneData('../precomputes/hc_gain_coefs.dat', hc_gain_coefs, hc_gain_coefs_sz(1),'hard clipping gain coefs');
hc_pre_filter_mid_freq = 380;
hc_pre_filter_q =0.25;
[b0, a0] = second_order_BP(hc_pre_filter_mid_freq, Fs, hc_pre_filter_q);
fsaver.saveIIR('../precomputes/hc_pre_filter_coefs.dat', toDspicQ16(b0), toDspicQ16(a0), 2, 1, 'hard clipping filter coefs');
fa.freqRespCoefs(b0,a0,'log','hard clipping post filter');%Figure 1
hc_post_filter_mid_freq = 350;
hc_post_filter_q = 1.44;
[ b0,a0 ] = second_order_BP(hc_post_filter_mid_freq, Fs, hc_post_filter_q);
fsaver.saveIIR('../precomputes/hc_post_filter_coefs.dat', toDspicQ16(b0), toDspicQ16(a0), 2, 1, 'HC post filter coefs');
fa.freqRespCoefs(b0,a0, 'log','HC post filter');%Figure 2

break;

%Soft Clipping
sc_gain_coefs = [1;3;5;10;20;30;50;70;85;100];
for n=1:length(sc_gain_coefs) sc_gain_coefs(n) = toDspicQ16(sc_gain_coefs(n)); end;
sc_gain_coefs_sz =  size(sc_gain_coefs);
fsaver.savePlaneData('../precomputes/sc_gain_coefs.dat',sc_gain_coefs, sc_gain_coefs_sz(1),'soft clipping gain coefs');
sc_filter_cut_freq = 780;
sc_filter_q = 0.5;
[b0, a0] = second_order_BP(sc_filter_cut_freq, Fs, sc_filter_q);
fsaver.saveIIR('../precomputes/sc_filter_coefs.dat', toDspicQ16(b0), toDspicQ16(a0), 2, 1, 'soft clipping filter coefs');
fa.freqRespCoefs(b0,a0,'log','soft clipping post filter');%Figure 2

%Compression
comp_coefs = [1.0;2.0;2.5;3;3.5;4;4.5;5;5.5;6];
for n=1:length(comp_coefs) comp_coefs(n) = toDspicQ16(comp_coefs(n)); end;
comp_coefs_sz =  size(comp_coefs);
fsaver.savePlaneData('../precomputes/comp_coefs.dat', comp_coefs, comp_coefs_sz(1),'simple compressor coefs');

% %Precomputes for chain position = 1%%%
lp_cut_freq = 440;
bp_freqs = [160,200,250,300,400,600,1000,1200,1600,2300];
hp_cut_freq = 400;
lp_hp_gain = [0, -2, -4, -6,  -8, -10, -12, -14, -16, -18];
%bp_peak_gain = 10;

%LP filter
for n = 1:length(lp_hp_gain)
    [ b(:,n),a(:,n) ] = fltSO( 'shelving', 'Treble_Shelf',lp_cut_freq,lp_hp_gain(n),0.707,Fs);
    y(n,:) = filter(b(:,n),a(:,n),x); 
    b(:,n) = toDspicQ16(b(:,n));
    a(:,n) = toDspicQ16(a(:,n));    
end;
fsaver.saveIIR('../precomputes/lp_filter_coefs.dat', b, a, 2, 10, 'low pass filter coefs');
fa.freqResp(y, 'log','low pass filter');%Figure 3

%BP filter
%fsaver.savePlaneData('../precomputes/bp_filter_gain_coefs.dat',toDspicQ16(1/db2mag(bp_peak_gain + 3)), 1,'band pass filter gain coef');
for n = 1:length(bp_freqs)
    %[ b(:,n),a(:,n) ] = fltSO( 'peak', 0,bp_freqs(n),bp_peak_gain,0.5,Fs);
    [ b(:,n),a(:,n) ] = second_order_BP(bp_freqs(n), Fs, 0.707);
    y(n,:) = filter(b(:,n),a(:,n),x); 
    b(:,n) = toDspicQ16(b(:,n));
    a(:,n) = toDspicQ16(a(:,n)); 
end;
fsaver.saveIIR('../precomputes/bp_filter_coefs.dat', b, a, 2, 10, 'band pass filter coefs');
fa.freqResp(y, 'log','band pass filter');%Figure 4

%HP filter
for n = 1:length(lp_hp_gain)
    [ b(:,n),a(:,n) ] = fltSO( 'shelving', 'Base_Shelf',hp_cut_freq,lp_hp_gain(n),0.707,Fs);
    y(n,:) = filter(b(:,n),a(:,n),x); 
    b(:,n) = toDspicQ16(b(:,n));
    a(:,n) = toDspicQ16(a(:,n)); 
end;
fsaver.saveIIR('../precomputes/hp_filter_coefs.dat', b, a, 2, 10, 'high pass filter coefs');
fa.freqResp(y, 'log','high pass filter');%Figure 5

% %Precomputes for chain position = 2%%%

%Chorus
wave_table_sz = 3000;
chorus_del_line_sz = 280;
ss_noise = ssource(wave_table_sz, Fs);
ss_noise.noise(chorus_del_line_sz/2, 55);
ss_noise.plotData();%Figure 6
s1 = ss_noise.getData();
s1 = s1 + chorus_del_line_sz/2; %Add DC
chorus_int_part = fix(s1);
chorus_frac_part=toDspicQ15(s1-fix(s1));
fsaver.savePlaneData('../precomputes/chorus_wt_int.dat',chorus_int_part, 1,'chorus int wt');
fsaver.savePlaneData('../precomputes/chorus_wt_frac.dat',chorus_frac_part, 1,'chorus frac wt');

%Flange
flange_wt_size = 3000;
flange_depth = 30; 
flange_source = ssource(flange_wt_size, Fs);
flange_source.trigonometric(flange_depth/2, Fs/flange_wt_size,0,flange_depth/2);
s1 = flange_source.getData();
int_part=fix(s1);
s1 = s1 - fix(s1);
frac_part = toDspicQ15(s1);    
fsaver.savePlaneData('../precomputes/flange_wt_int.dat', int_part, 1,'flange int wt');
fsaver.savePlaneData('../precomputes/flange_wt_frac.dat', frac_part, 1,'flange fract wt');
flange_source.plotData();%Figure 7

