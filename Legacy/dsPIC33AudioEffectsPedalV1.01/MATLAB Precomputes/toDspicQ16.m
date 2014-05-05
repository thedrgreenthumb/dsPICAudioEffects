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

for n=1:length(x)
    
    if ((x(n) > 32768) || (x(n) < -32768))
        error('Input value mast be at range -32767..32767');
    end;

    if (x(n) >= 0)
        yFrac  = fix(65536*(x(n) - fix(x(n))));    
        y(n) = fix(x(n))*65536 +yFrac;
    else
        x(n) = -x(n);
        yFrac  = fix(65536*(x(n)-fix(x(n))));    
        y(n) = -1*(fix(x(n))*65536+yFrac);
    end

end

