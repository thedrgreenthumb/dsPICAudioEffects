function [ out ] = lfo(length, name, war, fs, f)
%LFO generate vector with quantity of elements = length var. 
%Value of elements has range from 0 to 1.
%
%fs - sampling frequency, f - frequency of LFO
%
%If variable name = 'trig'  - has bin generated elements of trigonometric fuction, 
%if war = 0 - sin, if war = 1 - cosine.
%
%If variable name = 'square' - has bin generated elements with value 0 or 1, 
%war value = pulse width/period.
%
%If variable name = 'trin' - has bin generated trinagular waveform,
%If war = 0 - simmetric tooth,
%If war = 1 - sawtooth,
%If war = 2 - reverse sawtooth.
%
%If name = 'nosie', has bin generated low frequency noise waveform.
%For war = 1:5, you can change HF components of waveform.
%170212

%Trigonometric functions%
if strcmp(name,'trig') == 1
	% If sin
	if war == 0
		p=0; % Sample counter
		for n=1:length
			p=p+1;
			out(n)=(sin(2*3.14*f*p/fs)+1)/2; 
			if p== fs/f
				p=0;
			end;
		end;  
	end;
	% If cosin
	if  war == 1
		p=0;
		for n=1:length
			p=p+1;
			out(n)=(cos(2*3.14*f*p/fs)+1)/2; 
			if p== fs/f
				p=0;
			end;
		end;  
	end;
end;
%Square waveform%
if strcmp(name,'square') == 1
	p=0;
	for n=1:length
		p=p+1;
		if p<war*(fs/f)
			out(n)=1;  
		else
			out(n)=0;
		end;
		if p== fs/f
			p=0;
		end; 
	end;  
end;
%Triangular wave%
if strcmp(name,'trin') == 1
	%if positive tooth
	if war == 1
		p=0;
		D=1;
		for n=1:length
			p=p+1;
			if p<D*(fs/f)
				out(n)=p/(D*(fs/f));  
			else
				out(n)=0;
			end;
			if p== fs/f
				p=0;
			end;
		end;
	end;
	%if negative tooth
	if war== 2
		p=0;
		D=1;
		for n=1:length
			p=p+1;
			if p<D*(fs/f)
				out(n)=1-p/(D*(fs/f));  
			else
				out(n)=0;
			end; 
			if p== fs/f
				p=0;
			end;   
		end;       
	end;
	%if symmetrical tooth
	if war == 0
		p=0;
		D=1;
		for n=1:length  
			p=p+1;
			if p<D*(fs/f)/2
				out(n)=2*(p/(D*(fs/f)));  
			else
				out(n)=2*(1-p/(D*(fs/f)));  
			end;
			if p== fs/f
				p=0;
			end;
		end;
	end;
end;
%Noise wave%
if strcmp(name,'noise') == 1
	p=0;
	for n=1:length
		p=p+1;
		switch war
			case 1
		out(n)=((sin(12*f*p/fs))*(sin(2*3.14*f*p/fs))+1)/1.9; %3 peaks per period
			case 2
		out(n)=((cos(12*f*p/fs))*(sin(12*f*p/fs))*(sin(2*3.14*f*p/fs))/1.1+0.5);%5 peaks per period
			case 3
			out(n)=((sin(10*f*p/fs))*(cos(10*f*p/fs))*(sin(10*f*p/fs))*(sin(22*f*p/fs))/0.85+0.5);%7 peaks per period
			case 4     
	   out(n)=((cos(f*p/fs))*(sin(20*f*p/fs))*(cos(20*f*p/fs))*(sin(20*f*p/fs))*(sin(2*3.14*f*p/fs))/0.75+0.55)/1.1;
		% 9 peaks per period
			case 5
	   out(n)=((cos(f*p/fs))*(sin(24*f*p/fs))*(cos(24*f*p/fs))*(sin(24*f*p/fs))*(sin(2*3.14*f*p/fs))/0.75+0.55)/1.1;
		%12 peaks per period   	
		end;
		if p== fs/f
			p=0;
		end;	
	end; 
end;

end

