
t.ByteOrder = 'littleEndian';
while t.BytesAvailable == 0
    %do nothing
end
 while t.BytesAvailable > 0
        header = fread(t, 1, 'char'); % 0.4 ms
        switch char(header)
            case 'b'
                batchNum = fread(t, 1, 'uint32');            
                fitness = fread(t, 1, 'double'); %0.4 ms
                fit = addsample(fit, 'Data', fitness, 'Time', batchNum); %1.5 ms
                disp(sprintf('ML %d: %f\n', batchNum, fitness))
                plot(fit); set(gca, 'YScale', 'log')
                clear header batchNum fitness
             case 'm'
                rows = fread(t, 1, 'uint32')
                cols = fread(t, 1, 'uint32')
                img = fread(t, rows*cols, 'double')
                img = (reshape(img, rows, cols))';
                image(img);
                clear rows cols
            case 'p'
                msgSize = fread(t, 1, 'uint16');
                disp(char(rot90(fread(t, msgSize, 'char'))));
                clear msgSize
            otherwise
                disp(sprintf('%s is not a recognized header character\n', char(header)))
        end
 end 

