
    N =60000;
    if ~exist('inputBatch', 'var')
       load digit_data.mat 
    end
 if ~exist('t', 'var') || strcmp(t.Status, 'closed')
     %close all
     t = connectNNE(6274 + 80);
      fits = [];
      res = [];
      batchNum = 1;
      
      datas = cell(1, 1);
      imgs = cell(1, 1);
      iter = 1;

 end
 
 tim = timer;
 tim.Period = 0.15;
 tim.StartDelay = 0.15;
 tim.ExecutionMode = 'fixedRate';
 tim.BusyMode = 'drop';
 tim.TimerFcn = @(~, ~) evalin('base', 'readNNE');
 start(tim);
 
 plotTimer = timer;
  plotTimer.Period = 60;
  plotTimer.StartDelay = 10;
  plotTimer.ExecutionMode = 'fixedRate';
  plotTimer.BusyMode = 'drop';
  plotTimer.TimerFcn = @(~, ~) evalin('base', 'plotTimerCallback');%'sendNNE(t,uint8(''p'')); plotNow(fits(:,1), [fits(:,2), movmean(fits(:,2), 100), fits(:,3)]); imgNow({datas{batchNum, :}}, (reshape(inputBatch(batchNum, :), [28 28]))'');'); %imgNow({255*datas{batchNum, 1}, 255*datas{batchNum, 2}, 255*datas{batchNum, 3} , 255*datas{batchNum, 4}, 255*datas{batchNum, 5}, 255*datas{batchNum, 6}, 255*datas{batchNum, 7} , 255*datas{batchNum, 8}, 255*datas{batchNum, 9}, 255*datas{batchNum, 10}, 255*datas{batchNum, 11} , 255*datas{batchNum, 4}});');
 start( plotTimer);
 
try
 msg = zeros(1, 784*8 + 1 + 10*8, 'uint8');
 msg(1) = uint8('t');
 sendNNE(t,uint8('p'));
%msg = uint8('t');
 for n = 1:N
     if mod(n/(N - mod(N,100))*100, 10) == 0
         disp([num2str(round(n/N*100)) '% sent'])
     end
     

     for i = 1:784
        % msg = [msg, swapbytes(typecast(double(inputBatch(n,i)), 'uint8'))];
        msg((i-1)*8+2:((i-1)*8)+9) =   typecast(double(inputBatch(n,i)), 'uint8');
     end
      for i = 1:10
        msg((i-1+784)*8+2:((i-1+784)*8)+9) = typecast(double(outputBatch(n,i)), 'uint8');

       %  msg = [msg, swapbytes(typecast(double(outputBatch(n,i)), 'uint8'))];
      end

    % msg = typecast(msg, 'uint8');

     while (length(datas) + 50) < n
        % pause(2);
     end
     sendNNE(t, msg);

     
     %readNNE;
 end
 %readNNE;
%postPlotter;
catch err
 stop;
 
 rethrow(err);
end

tic();
     while length(datas) < N
         pause(2);
     end
     disp(num2str(toc()) );
  stop;   
  postPlotter;

     
