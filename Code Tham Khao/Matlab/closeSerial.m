%close Serial
pause(1);
clc
clear all
if ~isempty(instrfind)
    fclose(instrfind);
    delete(instrfind);
end
close all;
clc
disp('Serial Port Closed');