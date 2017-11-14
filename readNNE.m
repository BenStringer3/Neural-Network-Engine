


 while t.BytesAvailable > 0
     %disp('in readNNE and bytes available')
     t.ByteOrder = 'littleEndian';
        header = fread(t, 1, 'char'); % 0.4 ms
        switch char(header)
            case 'b'
                batchNum = fread(t, 1, 'uint32');            
                fitness = fread(t, 1, 'double'); %0.4 ms
                fits = [fits; batchNum, fitness]; %addsample(fits, 'Data', fitness, 'Time', batchNum); %1.5 ms
                %disp(sprintf('ML %d: %f\n', batchNum, fitness))
                if mod(batchNum/(N - mod(N,100))*100, 10) == 0
                     disp([num2str(round(batchNum/N*100)) '% received'])
                end
                clear header batchNum fitness
             case 'm'
                rows = fread(t, 1, 'uint32');
                cols = fread(t, 1, 'uint32');
                mat = fread(t, rows*cols, 'double');
                mat = (reshape(mat, rows, cols))';
                res = [res; mat];
                %image(myImgAx, mat);
                clear rows cols
             case 'r'
                rows = fread(t, 1, 'uint32');
                cols = fread(t, 1, 'uint32');
                mat = fread(t, rows*cols, 'double');
                mat = (reshape(mat, rows, cols))';
                %img = [img; mat];
                image(ax1, mat);
                image(ax2, (reshape(inputBatch(length(res) , :) , [28 28]))');
                clear rows cols
            case 'p'
                msgSize = fread(t, 1, 'uint16');
                disp(char(rot90(fread(t, msgSize, 'char'))));
                clear msgSize
            otherwise
                disp(sprintf('%s is not a recognized header character\n', char(header)))
        end
 end 


