%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FileName:        cmTimePlots.m
% FileVersion      1.01
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
function cmTimePlots(Y1,Y2,FS,type)
%This function plots time depended data.
%
%Y1 - 1 input.
%Y2 - 2 input.
%if type = 'samp', x-axis will be in samples, if 'sec' - in seconds.
%
%270113

% Create figure
figure1 = figure;

% Create axes
axes1 = axes('Parent',figure1);
box(axes1,'on');
grid(axes1,'on');
hold(axes1,'all');

%Vectors initialization
if((strcmp(type,'samp') ~= 1) && (strcmp(type,'sec') ~= 1))
    error(['Unsupported x-axis type: ' type]);
end;

if((strcmp(type,'samp') == 1))    
    stem((0:length(Y1)-1), Y1,'Marker','none'); 
    stem((0:length(Y2)-1),Y2, 'Color', [1 0 0],'Marker','none');
end;

if((strcmp(type,'sec') == 1))
    plot((0:length(Y1)-1)/FS, Y1,'Marker','none'); %1 channel
    plot((0:length(Y2)-1)/FS,Y2, 'Color', [1 0 0],'Marker','none');%2 channel has red color
end;

% Create title
title('Time plot');

% Create xlabel
if((strcmp(type,'samp') == 1))    
    xlabel('n->');
end;
if((strcmp(type,'sec') == 1))
    xlabel('s->');
end;

% Create ylabel
ylabel('1,2(red)');

