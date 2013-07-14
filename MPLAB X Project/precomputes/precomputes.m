% FileName:        wave_tables_gen.m
% Dependencies:    -
% 
%MATLAB v:       7.12.0 (R2011a)
% 
% Design in:        SAL
% Design by:        
% Feedback:                 
%                           150812
% 
%  ADDITIONAL NOTES:
%  -


clear all;
close all;


%Delay line length
DEPTH=50;

%Generate sine wavetable:

%First input parameter of lfo.m function - quantity of elements in wavetable
%Second input parameter of lfo.m has a "string" type - wavetable type
%If 0 - sine wave table will generate
%1000 - sampling frequency
%1 - frequency of signal.
%We generate 1000 sine wavetable values with 1 period
%For more information see lfo.m
%s1 = DEPTH*lfo(3000, 'trig', 0, 3000, 1);
s1 = ifNoiseGen('lowpass', 100 ,400 , 3000 , 20000);

s1 = 30 * s1;

%Get integer values of wavetable and write to file
int_part=fix(s1);
dlmwrite('int_part.dat',int_part);


%Get fractional values of wave table and write to file 
frac_part(length(s1))=0;

for n=1:length(s1)
    frac_part(n)=fix(32768*(s1(n)-fix(s1(n))));    
end;

dlmwrite('frac_part.dat',frac_part);





