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
%                       dsPIC33 Project
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

Fs= 20000;

len = 10000;
x(len) = 0;
x(1) = 1;
y(len,10) = 0;

%Precomputes for filters, chain position = 1
Flp = 400;
Fbp = [160,200,250,300,400,600,800,1200,1600,2000];
Fhp = 400;
Glp_hp = [0, -2, -4, -6,  -7, -8, -9, -10,-12, -14];
Gpeak = 6;

b(3,10) = 0;
a(3,10) = 0;

%LP filter
for n = 1:length(Glp_hp)
    [ b(:,n),a(:,n) ] = fltSO( 'shelving', 'Treble_Shelf',Flp,Glp_hp(n),0.707,Fs);
    y(:,n) = filter(b(:,n),a(:,n),x); 
end;

%Figure1
cmMagResp(y,0,Fs, 'log');
coefs = [a,b];
dlmwrite('../MPLAB X Project/precomputes/lpfilter.dat',coefs);

%BP filter
for n = 1:length(Fbp)
    [ b(:,n),a(:,n) ] = fltSO( 'peak', 0,Fbp(n),Gpeak,0.707,Fs);
    y(:,n) = filter(b(:,n),a(:,n),x); 
end;

%Figure2
cmMagResp(y,0,Fs, 'log');
coefs = [a,b];
dlmwrite('../MPLAB X Project/precomputes/bpfilter.dat',coefs);

%HP filter
for n = 1:length(Glp_hp)
    [ b(:,n),a(:,n) ] = fltSO( 'shelving', 'Base_Shelf',Fhp,Glp_hp(n),0.707,Fs);
    y(:,n) = filter(b(:,n),a(:,n),x); 
end;

%Figure3
cmMagResp(y,0,Fs, 'log');
coefs = [a,b];
dlmwrite('../MPLAB X Project/precomputes/hpfilter.dat',coefs);

%Precomputes for modulators, chain position = 2

Lchor = 300;
Lfl = 30;
Nsamples = 3000;

%Chorus
s1 = ifNoiseGen('lowpass', 30 ,200 ,Nsamples , Fs);
s1 = Lchor * s1;
int_part=fix(s1);

frac_part(length(s1))=0;
for n=1:length(s1)
    frac_part(n)=fix(32768*(s1(n)-fix(s1(n))));    
end;

%Figure 4
cmTimePlots(Lchor*frac_part/32768,int_part,Fs,'sec');
s = [int_part, frac_part];
dlmwrite('../MPLAB X Project/precomputes/chorus.dat',s);


%Flange
s1 = ifOscillator('harm', [0,0], 1, Fs/Nsamples, Nsamples, Fs);
s1 = Lfl * s1;
int_part=fix(s1);

frac_part(length(s1))=0;
for n=1:length(s1)
    frac_part(n)=fix(32768*(s1(n)-fix(s1(n))));    
end;

%Figure 5
cmTimePlots(Lfl*frac_part/32768,int_part,Fs,'sec');
s = [int_part, frac_part];
dlmwrite('../MPLAB X Project/precomputes/flange.dat',s);




