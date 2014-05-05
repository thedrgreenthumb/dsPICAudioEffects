%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FileName:        ifNoiseGen.m
% FileVersion      1.01
%
% MATLAB v:        8.0.0 (R2012b)
%
% Design by:       SAL
% Feedback:
%				   300213
%
% ADDITIONAL NOTES:
% ifXxx - group of influence functions.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [ y ] = ifNoiseGen(Type , Lf , Hf , Length , Fs)
%ifNoiseGen() generate vector of filtered white noise. 
%
%Type = filter type ('lowpass','highpass','bandpass','notch')
%Lf   = lowpass cutoff frequency in Hz.
%Hf   = highpass cutoff frequency in Hz.
%Length  = number of samples.
%Fs   = sample rate in Hz.
%
%y = filtered noise segment of length dur as specified by the inputs and have range from 0 to 1.
%
%Based at  www.h6.dion.ne.jp/~fff/old/technique/auditory/matlab.html
%260312

%Wrapping
type = Type;
lf = Lf;
hf = Hf;
dur  = Length/Fs;
sr = Fs;

%Error detecting
if (strcmp(type,'lowpass') ~= 1) && (strcmp(type,'highpass') ~= 1) && (strcmp(type,'bandpass') ~= 1) && (strcmp(type,'notch') ~= 1)
error('unknown input string argument');
end;

n = ceil(sr*dur) ;
lp = round(lf*dur);
hp = round(hf*dur);
switch type
  case 'lowpass'
     filter=zeros(1,n);
     filter(1,1:lp) = 1;
     filter(1,n-lp:n) = 1;
  case 'highpass'
     filter = ones(1,n);
     filter(1,1:hp) = 0;
     filter(1,n-hp:n) = 0;
  case 'bandpass'
     filter = zeros(1,n);
     filter(1,lp:hp) = 1;
     filter(1,n-hp:n-lp) = 1;
  case 'notch'
     filter = ones(1,n);
     filter(1,lp:hp) = 0;
     filter(1,n-hp:n-lp) = 0;
  otherwise
     error( 'bad type' ) ;
end

stream = RandStream.getGlobalStream ;
noise = rand( stream , 1 , n ) ;
noise = noise - mean( noise ) ;
y = real( ifft( fft( noise ) .* filter ) );
y = ((y / max( abs( y ) ))+1)/2 ;

end

