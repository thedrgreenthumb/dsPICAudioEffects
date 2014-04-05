%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FileName:        second_order_BP.m
% FileVersion      1.01
%
% MATLAB v:        8.0.0 (R2012b)
%
% Design by:       SAL
% Feedback:
%				   300213
%
% ADDITIONAL NOTES:
% 	See below function declaration.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [b, a] = second_order_BP(fc, fs, Q)
%SECOND_ORDER_BP - Second-Order IIR Butterworth Bandpass Filter Design Routine
%
%     usage:  [b, a] = second_order_BP(fc, fs, Q)
%     
%           b = Three column vector, numerator coefficients
%           a = Three column vector, denominator coefficients
%          fc = Scalar, center frequency (in Hz)
%          fs = Scalar, sampling frequency (in Hz)
%           Q = Scalar, quality factor (default: 1)

%  NOTICE: MATLAB's filter.m function implements the standard 
%          difference equation with subtracting the weighted
%          previous outputs:
%
%          a(1)*y(n) = b(1)*x(n) + b(2)*x(n-1) + b(3)*x(n-2)
%                                - a(2)*y(n-1) - a(3)*y(n-2) 
%
%          Therefore this routine negates the calculated a(2) and 
%          a(3) coefficients in order to accomodate this. However,
%          if the difference equation of your implementation adds 
%          the weighted previous outputs rather than subtracting 
%          them then you should use conv_coeff.m function to
%          obtain the original coefficients.

%   References:
%      [1]  John Lane, Jayant Datta, Brent Karley, Jay Norwood, "DSP Filters", 
%           PROMPT Publications(an imprint of Sams Technical Publishing), 
%           Indianapolis, IN, 2001, page 84.
%           Book's website: www.dspaudiocookbook.com

%  Copyright (c) 2005 Bo�a� Topakta�
%  Revision: 1.0      Date: 13 May 2005 18:19
%  This code may be used and modified freely provided 
%  that credit is given to the author in any public release.  
%  It is provided AS IS and without warranty of any kind.
if nargin < 3
    Q = 1;
end
beta = 0.5 * ( ( 1 - tan((2 * pi * (fc / fs)) / (2 * Q)) ) / ( 1 + tan((2 * pi * (fc / fs)) / (2 * Q)) ) );
gamma = ( 0.5 + beta ) * cos ( 2 * pi * (fc / fs) );
alfa = ( 0.5 - beta ) / 2;
a = zeros(1,3);
b = zeros(1,3);
a(1) = 1;
a(2) = -2*gamma; % notice the sign!
a(3) = 2*beta;   % notice the sign!
b(1) = 2*alfa;
b(2) = 0;
b(3) = -2*alfa;