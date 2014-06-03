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

Fs = 20000;

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
hc_gain_coefs = [1;10;20;50;100;150;200;250;300;400];
for n=1:length(hc_gain_coefs) hc_gain_coefs(n) = toDspicQ16(hc_gain_coefs(n)); end;
hc_gain_coefs_sz =  size(hc_gain_coefs);
fsaver.savePlaneData('../precomputes/hc_gain_coefs.dat', hc_gain_coefs, hc_gain_coefs_sz(1),'hard clipping gain coefs');
hc_filter_cut_freq = 380;
hc_filter_q =0.5;
[b0, a0] = second_order_BP(hc_filter_cut_freq, Fs, hc_filter_q);
fsaver.saveIIR('../precomputes/hc_filter_coefs.dat', toDspicQ16(b0), toDspicQ16(a0), 2, 1, 'hard clipping filter coefs');
fa.freqRespCoefs(b0,a0,'log','hard clipping post filter');%Figure 1

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

% % %Precomputes for chain position = 1%%%
% lp_cut_freq = 440;
% bp_freqs = [160,200,250,300,400,600,1000,1200,1600,2300];
% hp_cut_freq = 400;
% lp_hp_gain = [0, -2, -4, -6,  -8, -10, -12, -14, -16, -18];
% bp_peak_gain = 10;
% 
% %LP filter
% for n = 1:length(lp_hp_gain)
%     [ b(:,n),a(:,n) ] = fltSO( 'shelving', 'Treble_Shelf',lp_cut_freq,lp_hp_gain(n),0.707,Fs);
%     y(n,:) = filter(b(:,n),a(:,n),x); 
%     b(:,n) = toDspicQ16(b(:,n));
%     a(:,n) = toDspicQ16(a(:,n));    
% end;
% fsaver.saveIIR('../precomputes/lp_filter_coefs.dat', b, a, 2, 10, 'low pass filter coefs');
% fa.freqResp(y, 'log','low pass filter');%Figure 3
% 
% %BP filter
% fsaver.savePlaneData('../precomputes/bp_filter_gain_coefs.dat',toDspicQ16(1/db2mag(bp_peak_gain)), 1,'band pass filter gain coef');
% for n = 1:length(bp_freqs)
%     [ b(:,n),a(:,n) ] = fltSO( 'peak', 0,bp_freqs(n),bp_peak_gain,0.707,Fs);
%     y(n,:) = filter(b(:,n),a(:,n),x); 
%     b(:,n) = toDspicQ16(b(:,n));
%     a(:,n) = toDspicQ16(a(:,n)); 
% end;
% fsaver.saveIIR('../precomputes/bp_filter_coefs.dat', b, a, 2, 10, 'band pass filter coefs');
% fa.freqResp(y, 'log','band pass filter');%Figure 4
% 
% %HP filter
% for n = 1:length(lp_hp_gain)
%     [ b(:,n),a(:,n) ] = fltSO( 'shelving', 'Base_Shelf',hp_cut_freq,lp_hp_gain(n),0.707,Fs);
%     y(n,:) = filter(b(:,n),a(:,n),x); 
%     b(:,n) = toDspicQ16(b(:,n));
%     a(:,n) = toDspicQ16(a(:,n)); 
% end;
% fsaver.saveIIR('../precomputes/hp_filter_coefs.dat', b, a, 2, 10, 'high pass filter coefs');
% fa.freqResp(y, 'log','high pass filter');%Figure 5
% 
% % % %Precomputes for chain position = 2%%%
% mod_buf_sz = 600;
% dlmwrite('../precomputes/mod_effects_buf_sz.dat',mod_buf_sz);
% wave_table_sz = 3000;
% 
% %Chorus
% chorus_del_line_sz = 298;
% chorus_in_coefs = [0.7,0.3];
% chorus_fb_coefs = [0.17,0.15];
% chorus_ff_coefs = [0.15,0.17];
% chorus_tap_szs = [fix(chorus_del_line_sz/3),fix(chorus_del_line_sz/2)];
% ss_noise = ssource(wave_table_sz, Fs);
% ss_noise.noise(chorus_del_line_sz, 55);
% ss_noise.plotData();%Figure 6
% s1 = ss_noise.getData();
% chorus_int_part = fix(s1);
% chorus_frac_part=toDspicQ15(s1-fix(s1));
% chorus_coefs = [chorus_in_coefs, chorus_fb_coefs, chorus_ff_coefs];
% fsaver.savePlaneData('../precomputes/chorus_coefs.dat',toDspicQ15(chorus_coefs), 3,'chorus alpass filters coefs');
% chorus_wave_table = [chorus_int_part, chorus_frac_part];
% fsaver.savePlaneData('../precomputes/chorus_wave_table.dat',chorus_wave_table, 2,'chorus wave table');

% %Flange
% Lfl = 30;
% flangeInputCoef = 0.3;
% flangeFeedForwardCoef = 0.5;
% flangeFeedbackCoef = 0.3;
% flangeTapLen = Lfl/2;
% s1 = ifOscillator('harm', [0,Lfl/2], Lfl/2, Fs/WaveTableLength, WaveTableLength, Fs);
% intPart=fix(s1);
% s1 = s1 - fix(s1);
% fracPart = toDspicQ15(s1);    
% s = [intPart, fracPart,Lfl,flangeTapLen,toDspicQ15(flangeInputCoef), ...
%     toDspicQ15(flangeFeedForwardCoef),toDspicQ15(flangeFeedbackCoef)];
% dlmwrite('../MPLAB X Project/precomputes/flange.dat',s);
% cmTimePlots(Lfl*fracPart/32768,intPart,Fs,'sec');%Figure 7
% 
% %Precomputes for chain position = 3%%%
% mainDelayBufLen = 6000;
% dlmwrite('../MPLAB X Project/precomputes/delay_effects_buf.dat',mainDelayBufLen);
% %Delay
% delayFFCoef = 0.5;
% delayTapCoef = 0.5;
% delayBufLen = mainDelayBufLen;
% s=[toDspicQ15(delayFFCoef),toDspicQ15(delayTapCoef), delayBufLen];
% dlmwrite('../MPLAB X Project/precomputes/delay.dat',s);
% %Echo
% echoFFCoef = 0.3;
% echoFBCoef = 0.3;
% echoTapCoef = 0.3;
% echoBufLen = mainDelayBufLen;
% s=[toDspicQ15(echoFFCoef),toDspicQ15(echoFBCoef),toDspicQ15(echoTapCoef), echoBufLen];
% dlmwrite('../MPLAB X Project/precomputes/echo.dat',s);
% % %Reverb
% reverbFFCoef = 0.5;
% reverbFBCoef = 0.25;
% reverbTapCoefs = [0.20,-0.25,0.15,-0.11];
% reverbTapLens = [1521,2963,3859];
% reverbAPFiltersCoefs = [0.23,0.17];
% reverbAPFiltersLens = [357,129];
% echoBufLen = mainDelayBufLen - reverbAPFiltersLens(1) - reverbAPFiltersLens(2);
% s=[toDspicQ15(reverbFFCoef), toDspicQ15(reverbFBCoef),toDspicQ15(reverbTapCoefs), ...
%     reverbTapLens, toDspicQ15(reverbAPFiltersCoefs),echoBufLen,reverbAPFiltersLens];
% dlmwrite('../MPLAB X Project/precomputes/reverb.dat',s);
% 
% 
