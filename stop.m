

stop(tim);
try
stop(tim2);
catch 
end
try
stop(plotTimer);
clear plotNow
clear imgNow
catch 
end

sendNNE(t, uint8('x'));
fclose(t);

