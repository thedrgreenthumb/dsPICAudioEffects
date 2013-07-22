%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FileName:        precomputes.m
% FileVersion      1.01
%
% MATLAB v:        8.0.0 (R2012b)
%
% Design by:       SAL
% Feedback:
%				  
%
% ADDITIONAL NOTES:
%                        dsPIC33 Pedal Project
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

Fs= 20000;

len = 10000;
x(len) = 0;
x(1) = 1;
y(len,10) = 0;

b0(3) = 0;
a0(3) = 0;
b(3,10) = 0;
a(3,10) = 0;

%Precomputes for chain position = 0%%%
%Hard Clipping
FHCFilter = 420;
QHCFilter =1;
HCControlCoefs = [10,20,30,50,70,100,150,200,250,300];
[b0, a0] = second_order_BP(FHCFilter, Fs, QHCFilter);
s0 = [toDspicQ16(b0),toDspicQ16(a0),toDspicQ16(HCControlCoefs)];
dlmwrite('../MPLAB X Project/precomputes/hard_clipping.dat',s0,'precision', 10);
cmMagResppz(b0,a0,Fs, 'log');%Figure 1
%Soft Clipping
FSCFilter = 340;
QSCFilter = 0.707;
SCControlCoefs = [1,2,3,5,10,30,50,100,300,500];
[b0, a0] = second_order_BP(FSCFilter, Fs, QSCFilter);
s0 = [toDspicQ16(b0),toDspicQ16(a0),toDspicQ16(SCControlCoefs)];
dlmwrite('../MPLAB X Project/precomputes/soft_clipping.dat',s0,'precision', 10);
cmMagResppz(b0,a0,Fs, 'log');%Figure 2
%Compression
CompControlCoefs = [1.0,2.0,3,4,5,6,7,8,9,10];
s0 = [toDspicQ16(CompControlCoefs)];
dlmwrite('../MPLAB X Project/precomputes/compression.dat',s0,'precision', 10);

% %Precomputes for chain position = 1%%%
% NumberOfCoefsSets = 10;
% Flp = 400;
% Fbp = [160,200,250,300,400,600,800,1200,1600,2000];
% Fhp = 400;
% Glp_hp = [0, -2, -4, -6,  -7, -8, -9, -10,-12, -14];
% Gpeak = 6;
% %LP filter
% for n = 1:length(Glp_hp)
%     [ b(:,n),a(:,n) ] = fltSO( 'shelving', 'Treble_Shelf',Flp,Glp_hp(n),0.707,Fs);
%     y(:,n) = filter(b(:,n),a(:,n),x); 
% end;
% coefs = [toDspicQ16(a(1,:)),toDspicQ16(a(2,:)),toDspicQ16(a(3,:)), ...
%     toDspicQ16(b(1,:)),toDspicQ16(b(2,:)),toDspicQ16(b(3,:)), ...
%     NumberOfCoefsSets];
% dlmwrite('../MPLAB X Project/precomputes/lpfilter.dat',coefs,'precision', 10);
% cmMagResp(y,0,Fs, 'log');%Figure 3
% %BP filter
% for n = 1:length(Fbp)
%     [ b(:,n),a(:,n) ] = fltSO( 'peak', 0,Fbp(n),Gpeak,0.707,Fs);
%     y(:,n) = filter(b(:,n),a(:,n),x); 
% end;
% coefs = [toDspicQ16(a(1,:)),toDspicQ16(a(2,:)),toDspicQ16(a(3,:)), ...
%     toDspicQ16(b(1,:)),toDspicQ16(b(2,:)),toDspicQ16(b(3,:)), ...
%     toDspicQ16(db2mag(Gpeak)),NumberOfCoefsSets];
% dlmwrite('../MPLAB X Project/precomputes/bpfilter.dat',coefs,'precision', 10);
% cmMagResp(y,0,Fs, 'log');%Figure 4
% %HP filter
% for n = 1:length(Glp_hp)
%     [ b(:,n),a(:,n) ] = fltSO( 'shelving', 'Base_Shelf',Fhp,Glp_hp(n),0.707,Fs);
%     y(:,n) = filter(b(:,n),a(:,n),x); 
% end;
% coefs = [toDspicQ16(a(1,:)),toDspicQ16(a(2,:)),toDspicQ16(a(3,:)), ...
%     toDspicQ16(b(1,:)),toDspicQ16(b(2,:)),toDspicQ16(b(3,:)), ...
%     NumberOfCoefsSets];
% dlmwrite('../MPLAB X Project/precomputes/hpfilter.dat',coefs,'precision', 10);
% cmMagResp(y,0,Fs, 'log');%Figure 5
% 
% %Precomputes for chain position = 2%%%
% mainModBufLen = 600;
% WaveTableLength = 3000;
% %Chorus
% Lchor = mainModBufLen/2;
% chorusInputCoefs = [0.7,0.5];
% chorusFeedbackCoefs = [0.5,0.5];
% chorusTapLens = [fix(Lchor/2),fix(Lchor/2)];
% s1 = ifNoiseGen('lowpass', 30 ,200 ,WaveTableLength , Fs);
% s1 = Lchor * s1;
% intPart = fix(s1);
% fracPart=toDspicQ15(s1-fix(s1));    
% s = [intPart, fracPart,Lchor,chorusTapLens,chorusInputCoefs,chorusFeedbackCoefs,WaveTableLength];
% dlmwrite('../MPLAB X Project/precomputes/chorus.dat',s);
% cmTimePlots(Lchor*fracPart/32768,intPart,Fs,'sec');%Figure 6
% %Flange
% Lfl = 30;
% flangeInputCoef = 0.5;
% flangeFeedForwardCoef = 0.7;
% flangeFeedbackCoef = 0.5;
% flangeTapLen = Lfl/2;
% s1 = ifOscillator('harm', [0,0], 1, Fs/WaveTableLength, WaveTableLength, Fs);
% s1 = Lfl * s1;
% intPart=fix(s1);
% s1 = s1 - fix(s1);
% fracPart = toDspicQ15(s1);    
% s = [intPart, fracPart,Lfl,flangeTapLen,flangeInputCoef,flangeFeedForwardCoef,flangeFeedbackCoef];
% dlmwrite('../MPLAB X Project/precomputes/flange.dat',s);
% cmTimePlots(Lfl*fracPart/32768,intPart,Fs,'sec');%Figure 7
% 
% %Precomputes for chain position = 3%%%
% mainDelayBufLen = 6000;
% %Delay
% delayTapCoef = 0.5;
% delayBufLen = mainDelayBufLen;
% s=[toDspicQ15(delayTapCoef), delayBufLen];
% dlmwrite('../MPLAB X Project/precomputes/delay.dat',s);
% %Echo
% echoTapCoef = 0.5;
% echoBufLen = mainDelayBufLen;
% s=[toDspicQ15(echoTapCoef), echoBufLen];
% dlmwrite('../MPLAB X Project/precomputes/echo.dat',s);
% %Reverb
% reverbTapCoefs = [0.7,0.5,0.3,0.22];
% reverbTapLens = [1521,2963,4497];
% reverbAPFiltersCoefs = [0.18,0.12];
% reverbAPFiltersLens = [357,129];
% echoBufLen = mainDelayBufLen - reverbAPFiltersLens(1) - reverbAPFiltersLens(2);
% s=[toDspicQ15(reverbTapCoefs), reverbTapLens, toDspicQ15(reverbAPFiltersCoefs),echoBufLen,reverbAPFiltersLens];
% dlmwrite('../MPLAB X Project/precomputes/reverb.dat',s);


