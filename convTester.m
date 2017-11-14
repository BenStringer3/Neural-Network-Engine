
    N = 1000;
 if ~exist('t', 'var') || strcmp(t.Status, 'closed')
     close all
     t = connectNNE(6274 + 80);
      fits = [];
      res = [];
      fig1 = figure();
      fig2 = figure();
      ax1 = axes(fig1);
      ax2 = axes(fig2);
 end
 
 tim = timer;
 tim.Period = 0.5;
 tim.StartDelay = 0.5;
 tim.ExecutionMode = 'fixedRate';
 tim.BusyMode = 'drop';
 tim.TimerFcn = @(~, ~) evalin('base', 'readNNE');
 start(tim);
 
try

 for n = 1:N
     if mod(n/(N - mod(N,100))*100, 10) == 0
         disp([num2str(round(n/N*100)) '% sent'])
     end
     msg = uint8('t');%, swapbytes(typecast(uint32(1), 'uint8')), swapbytes(typecast(uint32(1), 'uint8'))];

     for i = 1:784
        msg = [msg,  swapbytes(typecast(double(inputBatch(n,i)), 'uint8'))];
     end
      for i = 1:10
        msg = [msg,  swapbytes(typecast(double(outputBatch(n,i)), 'uint8'))];
     end
 
     msg = typecast(msg, 'uint8');
     

     while (length(res) + 5) < n
        % pause(2);
     end
     sendNNE(t, msg);

     
     %readNNE;
 end
 readNNE;
postPlotter;
catch err
 stop;
 
 rethrow(err);
end
tic();
     while length(res) < n
        % pause(2);
     end
     disp(num2str(toc()) );
  stop;   

     
