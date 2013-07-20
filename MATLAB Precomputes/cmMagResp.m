%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FileName:        cmMagResp.m
% FileVersion      1.02
%
% MATLAB v:        8.0.0 (R2012b)
%
% Design by:       SAL
% Feedback:
%				   300213
%
% ADDITIONAL NOTES:
% cmXxx - group of compute functions.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function cmMagResp(Y1,Y2,FS, type)
%This function compute magnitude response plot from impulse response.
%
%Y1 - 1 input.
%Y2 - 2 input.
%FS - sample frequency.
%If type = 'lin', plot in lin scales, if 'log' - in log scales.
%
%130713

N=length(Y1);
if Y1== 0
N=length(Y2);
end;

a=zeros(1,N);
a(1)=1;

%Finding errors
if((strcmp(type,'lin') ~= 1) && (strcmp(type,'log') ~= 1))
    error(['Unsupported x-axis type: ' type]);
end;

%Calculating Magnitudes
mxsz = size(Y1);
if(mxsz(1) < mxsz(2))
    Y1 = reshape(Y1,mxsz(2),mxsz(1));
end;
mxsz = size(Y1);
for n=1:mxsz(2)
    [H1(:,n), F]=freqz(Y1(:,n),a,65536,FS);
end;

mxsz = size(Y2);
if(mxsz(1) < mxsz(2))
    Y2 = reshape(Y2,mxsz(2),mxsz(1));
end;
mxsz = size(Y2);
for n=1:mxsz(2)
    [H2(:,n), F]=freqz(Y2(:,n),a,65536,FS);
end;


% Create figure
figure1 = figure;

if((strcmp(type,'lin') == 1))
% Create lin axes
    axes1 = axes('Parent',figure1);
    box(axes1,'on');
    grid(axes1,'on');
    hold(axes1,'all');

    plot(F,(abs(H1)));
    plot(F,(abs(H2)),'Color', [1 0 0]);
end;

if((strcmp(type,'log') == 1))
% Create log axes
    axes1 = axes('Parent',figure1,'XScale','log','XMinorTick','on',...
        'XMinorGrid','on');
    box(axes1,'on');
    grid(axes1,'on');
    hold(axes1,'all');

    plot(F,10*log(abs(H1)));
    plot(F,10*log(abs(H2)),'Color', [1 0 0]);
end;

% Create title
title('Magnitude of discrete system based impulse response');

% Create xlabel
xlabel('f->');

% Create ylabel
ylabel('1,2(red)');

