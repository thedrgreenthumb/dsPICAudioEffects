%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FileName:        cmMagResppz.m
% Dependencies:    -
% 
% MATLAB v:        8.0.0 (R2012b)
% 
% Organization:    SAL
% Design by:       drgreenthumb
% Feedback:                 
%                  150812
% 
% 
%  ADDITIONAL NOTES:
%  cmXxx - group of compute functions.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function cmMagResppz( b,a,FS, type )
%This function compute magnitude response plot from filter coef's.
%
%cmXxxpz - group of compute functions, what uses poles and zeros.
%
%b - numerator.
%a - denumerator.
%FS - sample frequency.
%If type = 'lin', plot in lin scales, if 'log' - in log scales.
%
%120412


%Finding errors
if((strcmp(type,'lin') ~= 1) && (strcmp(type,'log') ~= 1))
    error(['Unsupported x-axis type: ' type]);
end;

%Calculating Magnitude
[H,F]=freqz(b,a,FS);

% Create figure
figure1 = figure;


if((strcmp(type,'lin') == 1))
% Create lin axes
    axes1 = axes('Parent',figure1);
    box(axes1,'on');
    grid(axes1,'on');
    hold(axes1,'all');

    plot((FS/2)*(F/pi),(abs(H)));
end;


if((strcmp(type,'log') == 1))
% Create log axes
    axes1 = axes('Parent',figure1,'XScale','log','XMinorTick','on',...
        'XMinorGrid','on');
    box(axes1,'on');
    grid(axes1,'on');
    hold(axes1,'all');

    plot((FS/2)*(F/pi),10*log(abs(H)));
end;

% Create title
title('Magnitude of discrete system based filter coefs');

% Create xlabel
xlabel('f->');

% Create ylabel
ylabel('magnitude');

end

