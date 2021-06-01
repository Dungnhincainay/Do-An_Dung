function [s,flag] = serialSetup(comport)
flag = 1;
s = serial(comport);
set(s,'DataBits',8);
set(s,'StopBits',1);
set(s,'BaudRate',9600);
set(s,'parity','none');




fopen(s);
a = 'b';
while (a ~= 'a')
    a=fread(s,1,'uchar')
end
if (a == 'a')
    disp('Serial read');
end
fprintf(s,'%c','a');
mbox = msgbox('Serial Communication setup'); % tao bang thong bao
uniwait(mbox);                               %doi den khi bang thong bao tat
fscanf(s,'%u')                               % lay gia tri unsigned decimal interger duoc gui tu arduino
end