n = 2;
 msg = uint8('t');%, swapbytes(typecast(uint32(1), 'uint8')), swapbytes(typecast(uint32(1), 'uint8'))];
 
 for i = 1:784
    msg = [msg,  swapbytes(typecast(double(inputBatch(n,i)), 'uint8'))];
 end
  for i = 1:10
    msg = [msg,  swapbytes(typecast(double(outputBatch(n,i)), 'uint8'))];
 end
 
     msg = typecast(msg, 'uint8');
     
 if ~exist('t', 'var') || strcmp(t.Status, 'closed')
     t = connectNNE(6274 + 80);
 end
     
     sendNNE(t, msg);
     
     readNNE;
     
     %sendNNE(t, uint8('x'));
    % fclose(t);
    % clear t
     
