% function sendTrainDat(tim2, inputBatch, outputBatch, t, N)
% persistent n;
% if isempty(n)
%    n=1; 
% end

if (n < batchNum + 7)
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

     sendNNE(t, msg);
     n = n + 1;
     drawnow;
end

if (n >= N)
   % stop;
   disp('finished sending');
   stop(tim2); 
end
