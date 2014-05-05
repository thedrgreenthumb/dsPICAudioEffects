%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FileName:        fltSO.m
% FileVersion      1.01
%
% MATLAB v:        8.0.0 (R2012b)
%
% Design by:       SAL
% Feedback:
%				   300213
%
% ADDITIONAL NOTES:
% fltXxx - filtering processing functions.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [ b,a ] = fltSO( Type, Val, f,G,Q,fs)
%This function is wrapper for second order filters compute functions.
%
% Type = 'shelving' or 'peak'. Describes types of filters.
% Val = 'Base_Shelf' or 'Treble_Shelf' if Type = 'shelving', 0 if Type ='peak'.
% f - frequency in Hz.
% G - Gain of filter i dB.
% Q - quality, optimal value = 0.707.
% fs - sampling frequency.
%
% [ b,a ] = computed coefficients.
%
if (strcmp(Type,'shelving'))
    [b, a]  = shelving(G, f, fs, Q, Val);
elseif(strcmp(Type,'peak') && Val == 0)
    [b, a]  = peak(G, f, fs, Q);
else
    error('Unexpected input arguments');
end;



function [b, a]  = shelving(G, fc, fs, Q, type)

%
% Derive coefficients for a shelving filter with a given amplitude and
% cutoff frequency.  All coefficients are calculated as described in
% Zolzer's DAFX book (p. 50 -55).  
%
% Usage:     [B,A] = shelving(G, Fc, Fs, Q, type);
%
%            G is the logrithmic gain (in dB)
%            FC is the center frequency
%            Fs is the sampling rate
%            Q adjusts the slope be replacing the sqrt(2) term
%            type is a character string defining filter type
%            Choices are: 'Base_Shelf' or 'Treble_Shelf'
%
% Author:    sparafucile17 08/22/05
%

%Error Check
if((strcmp(type,'Base_Shelf') ~= 1) && (strcmp(type,'Treble_Shelf') ~= 1))
    error(['Unsupported Filter Type: ' type]);
end

K = tan((pi * fc)/fs);
V0 = 10^(G/20);
root2 = 1/Q; %sqrt(2)

%Invert gain if a cut
if(V0 < 1)
    V0 = 1/V0;
end

%%%%%%%%%%%%%%%%%%%%
%    BASE BOOST
%%%%%%%%%%%%%%%%%%%%
if(( G > 0 ) && (strcmp(type,'Base_Shelf')))
   
    b0 = (1 + sqrt(V0)*root2*K + V0*K^2) / (1 + root2*K + K^2);
    b1 =             (2 * (V0*K^2 - 1) ) / (1 + root2*K + K^2);
    b2 = (1 - sqrt(V0)*root2*K + V0*K^2) / (1 + root2*K + K^2);
    a1 =                (2 * (K^2 - 1) ) / (1 + root2*K + K^2);
    a2 =             (1 - root2*K + K^2) / (1 + root2*K + K^2);

%%%%%%%%%%%%%%%%%%%%
%    BASE CUT
%%%%%%%%%%%%%%%%%%%%
elseif (( G < 0 ) && (strcmp(type,'Base_Shelf')))
   
    b0 =             (1 + root2*K + K^2) / (1 + root2*sqrt(V0)*K + V0*K^2);
    b1 =                (2 * (K^2 - 1) ) / (1 + root2*sqrt(V0)*K + V0*K^2);
    b2 =             (1 - root2*K + K^2) / (1 + root2*sqrt(V0)*K + V0*K^2);
    a1 =             (2 * (V0*K^2 - 1) ) / (1 + root2*sqrt(V0)*K + V0*K^2);
    a2 = (1 - root2*sqrt(V0)*K + V0*K^2) / (1 + root2*sqrt(V0)*K + V0*K^2);

%%%%%%%%%%%%%%%%%%%%
%   TREBLE BOOST
%%%%%%%%%%%%%%%%%%%%
elseif (( G > 0 ) && (strcmp(type,'Treble_Shelf')))

    b0 = (V0 + root2*sqrt(V0)*K + K^2) / (1 + root2*K + K^2);
    b1 =             (2 * (K^2 - V0) ) / (1 + root2*K + K^2);
    b2 = (V0 - root2*sqrt(V0)*K + K^2) / (1 + root2*K + K^2);
    a1 =              (2 * (K^2 - 1) ) / (1 + root2*K + K^2);
    a2 =           (1 - root2*K + K^2) / (1 + root2*K + K^2);

%%%%%%%%%%%%%%%%%%%%
%   TREBLE CUT
%%%%%%%%%%%%%%%%%%%%

elseif (( G < 0 ) && (strcmp(type,'Treble_Shelf')))

    b0 =               (1 + root2*K + K^2) / (V0 + root2*sqrt(V0)*K + K^2);
    b1 =                  (2 * (K^2 - 1) ) / (V0 + root2*sqrt(V0)*K + K^2);
    b2 =               (1 - root2*K + K^2) / (V0 + root2*sqrt(V0)*K + K^2);
    a1 =             (2 * ((K^2)/V0 - 1) ) / (1 + root2/sqrt(V0)*K + (K^2)/V0);
    a2 = (1 - root2/sqrt(V0)*K + (K^2)/V0) / (1 + root2/sqrt(V0)*K + (K^2)/V0);

%%%%%%%%%%%%%%%%%%%%
%   All-Pass
%%%%%%%%%%%%%%%%%%%%
else
    b0 = V0;
    b1 = 0;
    b2 = 0;
    a1 = 0;
    a2 = 0;
end

%return values
a = [  1, a1, a2];
b = [ b0, b1, b2];



function [b, a]  = peak(G, fc, fs, Q)

% Derive coefficients for a peak filter with a given amplitude and
% cutoff frequency.  All coefficients are calculated as described in
% Zolzer's DAFX book.  
%(100212)

K = tan((pi * fc)/fs);
V0 = 10^(G/20);

if( G > 0 ) 
%BOOST 
b0=(1+K*V0/Q+K^2)/(1+K/Q+K^2);

b1=(2*(K^2-1))/(1+K/Q+K^2);

b2=(1-K*V0/Q+K^2)/(1+K/Q+K^2);

a1=(2*(K^2-1))/(1+K/Q+K^2);

a2=(1-K/Q+K^2)/(1+K/Q+K^2);

else
 %CUT
b0=(1+K/Q+K^2)/(1+K/(V0*Q)+K^2);

b1=(2*(K^2-1))/(1+K/(V0*Q)+K^2);

b2=(1-K/Q+K^2)/(1+K/(V0*Q)+K^2);

a1=(2*(K^2-1))/(1+K/(V0*Q)+K^2);

a2=(1-K/(V0*Q)+K^2)/(1+K/(V0*Q)+K^2);
end;

%return values
a = [  1, a1, a2];
b = [ b0, b1, b2];


