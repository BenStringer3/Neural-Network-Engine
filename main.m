
    N =12000;
    n=1;
    batchNum = 1;
    if ~exist('inputBatch', 'var')
       load digit_data.mat 
    end
 if ~exist('t', 'var') || strcmp(t.Status, 'closed')
     %close all
     t = connectNNE(6274 + 80);
      fits = [];
      res = [];
      datas = cell(1, 1);
      iter = 1;

 end
 
 tim = timer;
 tim.Period = 0.1;
 tim.StartDelay = 0.1;
 tim.ExecutionMode = 'fixedRate';
 tim.BusyMode = 'drop';
 tim.TimerFcn = @(~, ~) evalin('base', 'readNNE');
 start(tim);
 
 tim2 = timer;
 tim2.Period = 0.05;
 tim2.StartDelay = 0.05;
 tim2.ExecutionMode = 'fixedRate';
 tim2.BusyMode = 'drop';
 tim2.TimerFcn = @(~, ~)evalin('base', 'sendTrainDat');% sendTrainDat(tim2, inputBatch, outputBatch, t, N);%evalin('base', 'sendTrainDat');
 start(tim2);
 
  
 
 %change learning rate
 stop(tim); stop(tim2); lrm = [uint8('l'), swapbytes(typecast(double(0.05), 'uint8'))]; lrm=typecast(lrm, 'uint8'); sendNNE(t, lrm); clear lrm;  start(tim); start(tim2);




     
