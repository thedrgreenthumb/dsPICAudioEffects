%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FileName:        ifOscillator.m
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
function [ Out ] = ifOscillator(Name, AddVar, Amps, Freqs, Length, Fs)
%ifOscillator() generate vectors of different oscillated functions. 
%
%Name - name of function.
%AddVar - additional variable.
%Amps - vector of amplitude values.
%Freqs - vector of frequency values.
%Length - length of output vector.
%Fs - sampling frequency.
%
%If variable Name = 'harm'  - has bin generated harmonic fuction
%AddVar = [phase in pi, DC value]
%If variable Name = 'meander' - has bin generated meander function
%AddVar(1) value = pulse width/period.
%If variable Name = 'trin' - has bin generated trinagular wave.
%If AddVar = 1 - simmetric tooth,
%If AddVar = 2 - sawtooth,
%If AddVar = 3 - reverse sawtooth.
%
%270113

%Errors checking
%Vectors lengthes
if (length(Amps) > Length)
    error('Vector of Amps must be less then Length');
end;
if (length(Freqs) > Length)
    error('Vector of Freqs must be less then Length');
end;
%Negative values in vectors search
for n = 1:length(Amps)
    if (Amps(n) < 0)
        error('Amps() must be more then 0')
    end;
end;
for n = 1:length(Freqs)
    if (Freqs(n) < 0)
        error('Freqs() must be more then 0')
    end;
end;

%Get AmpsVect, it length equal Length
AmpsVect(Length) = 0;
CurAmp = 0;
p = 1;
AmpsStep = Length/length(Amps); 
for m = 1:Length
    AmpsVect(m) = Amps(p);
    if ( m > AmpsStep * p)
        p = p + 1;
    end;
end;
%Get FreqsVect, it length equal Length
FreqsVect(Length) = 0;
CurFreq = 0;
p = 1;
FreqsStep = Length/length(Freqs); 
for m = 1:Length
    FreqsVect(m) = Freqs(p);
    if ( m > FreqsStep * p)
        p = p + 1;
    end;
end;

Out(Length) = 0;
%Trigonometric wave%
p = 0; %Sample counter
if strcmp(Name,'harm') == 1
    for n = 1:Length
        p = p + 1;
        Out(n) = AmpsVect(n) * sin(2*3.14*FreqsVect(n)*n/Fs + AddVar(1)) + AddVar(2); 
        if (p >= Fs/FreqsVect(n))
            p=0;
        end;
    end;
end;

%Square wave%
if strcmp(Name,'meander') == 1
    %Errors check
    if (length(AddVar) ~= 1)
            error('if name = meander, length(AddVar) must be equal 1');
    end;
    if (AddVar >= 1)
            error('if name = meander, AddVar must be less then 1');
    end;
    p = 0;
    for n = 1:Length   
        p = p + 1;
        if p < AddVar*(Fs/CurFreq)
            Out(n) = CurAmp;  
        else
            Out(n) = -CurAmp;
        end;
        if p >= Fs/FreqsVect(n)
            p = 0;
            CurAmp = AmpsVect(n);
            CurFreq = FreqsVect(n);
        end;
    end;  
end;

%Triangular wave%
if strcmp(Name,'trin') == 1
    %Errors checking
    if (length(AddVar) ~= 1 || AddVar < 1 || AddVar > 3)
        error('Name = trin, uncorrect AddVar value')
    end;
    %if septenary tooth
    if AddVar == 1
        p = 0;
        for n = 1:Length   
            p = p + 1;
            if p < (Fs/FreqsVect(n))/2
                Out(n) = CurAmp*2*p /(Fs/CurFreq);  
            else
                Out(n) = CurAmp*2*(1 - p/(Fs/CurFreq));  
            end;
            if (p >= Fs/FreqsVect(n))
                p = 0;
                CurAmp = AmpsVect(n);
                CurFreq = FreqsVect(n);
            end; 
            if (Out(n) < 0)%Facking Hardcode !!! 0_0
                Out(n) = 0;
            end;
        end;
    end;
    %if positive tooth
    if (AddVar == 2)
        p=0;
        for n = 1:Length
            p = p + 1; 
            if p < (Fs/FreqsVect(n))
                Out(n) = CurAmp*p /(Fs/CurFreq);  
            else
                Out(n) = 0;
            end;
            if p >= Fs/FreqsVect(n)
                p = 0;
                CurAmp = AmpsVect(n);
                CurFreq = FreqsVect(n);
            end;
        end;
    end;
    %if negative tooth
    if (AddVar == 3)
        p=0;
        for n = 1:Length
            p = p + 1;
            if p < (Fs/FreqsVect(n))
                Out(n) = CurAmp*(1 - p/(Fs/CurFreq));  
            else
                Out(n) = 0;
            end;   
            if p >= Fs/FreqsVect(n)
                p = 0;
                CurAmp = AmpsVect(n);
                CurFreq = FreqsVect(n);
            end;   
        end;       
    end;
end;

end

