%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FileName:        toDspicQ16.m
% FileVersion      1.01
%
% MATLAB v:        8.0.0 (R2012b)
%
% Design by:       SAL
% Feedback:
%				   300213
%
% ADDITIONAL NOTES:
% Microchip dsPIC controllers specific function.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [ y ] = toDspicQ16( x )

if ((x > 32768) || (x < -32768))
	error('Input value mast be at range -32767..32767');
end;
    
if (x>=0)
    yFrac  = fix(65536*(x-fix(x)));    
    y = fix(x)*65536 +yFrac;
else
    x = -x;
    yFrac  = fix(65536*(x-fix(x)));    
    y = -1*(fix(x)*65536+yFrac);
end

