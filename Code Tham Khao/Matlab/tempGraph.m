%tao do thi de hien thi nhiet do va gui len GUI
if (~exist('myAxes','var'))
    buf_len = 500;
    index = 1 : buf_len;
    zeroIndex = zeros(size(index));
    tcdata = zeroIndex;
    limits = [0 100];
    myAxes = axes('XLim',[0 buf_len],'YLim',[0 0.1],'ZLim',limits,'CLim',limits);
    view(0,0);
    grid on;
    s = surface(index,[0 0], [tcdata; zeroIndex], [tcdata; tcdata]);
    set(s,'EdgeColor','flat','FaceColor','flat','Parent',myAxes);
    colorbar
    set(handles.Temp_plot,'Position',[200 200 890 660]);
    drawnow;
end
  %hien thi nhiet do va cap nhat mau chp Plot  
    
   %mode = 'T'; 
    
    
    
    
    
    
    %mode = 'T';
    global mode startRun
    mode = 'R';
    tc = 0;
    while (startRun == 1)
        switch mode
            case 'T'
                tc = readTemp(arduino.s,mode);
                set(handles.Temp,'String',tc);
                tcdata = [tcdata(2:end),tc];
                set(s,'Zdata',[tcdata; zeroIndex],'Cdata',[tcdata: tcdata]);
                drawnow;
            case 'R'
                sTemp = get(handles.Temp_set,'String');
                sTemp = str2num(sTemp);
                Setpoint(arduino.s,mode,sTemp);
                set(handles.Temp,'String',tc);
                tcdata = [tcdata(2:end),tc];
                set(s,'Zdata',[tcdata; zeroIndex],'Cdata',[tcdata: tcdata]);
                drawnow;  
                mode = 'T';
            otherwise
                break;
        end