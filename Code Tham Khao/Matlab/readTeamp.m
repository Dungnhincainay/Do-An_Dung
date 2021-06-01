function output = readTemp(s,mode)
% gui tin hieu cho arduino doc nhiet do 
fprintf(s,mode);
%lay gia tri nhiet do tu arduino
output = fscanf(s,'%f');
end