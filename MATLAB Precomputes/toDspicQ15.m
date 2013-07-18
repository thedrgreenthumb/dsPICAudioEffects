%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FileName:        toDspicQ15.m
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
function [ y ] = toDspicQ15(x)

for n=1:length(x)

    if ((x(n) >= 1) || (x(n) <= -1))
        error('Input value mast be at range (-1,1)');
    end;

    if (x(n)>=0)
        y(n) = fix(32768*(x(n)-fix(x(n))));   
    else
        x(n) = -x(n);
        y(n) = -1*fix(32768*(x(n)-fix(x(n))));  
    end

end;