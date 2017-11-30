

try
 
 while t.BytesAvailable > 0
     %disp('in readNNE and bytes available')
     t.ByteOrder = 'littleEndian';
        header = fread(t, 1, 'char'); % 0.4 ms
        switch char(header)
            case 'b'
                batchNum = fread(t, 1, 'uint32');            
                fitness = fread(t, 1, 'double'); %0.4 ms
                learningRate = fread(t, 1, 'double');
                fits = [fits; batchNum, fitness, learningRate]; %addsample(fits, 'Data', fitness, 'Time', batchNum); %1.5 ms
                %disp(sprintf('ML %d: %f\n', batchNum, fitness))
                if mod(batchNum/(N - mod(N,100))*100, 10) == 0
                     disp([num2str(round(batchNum/N*100)) '% received'])
                end
                clear header  fitness
             case 'm'
                rows = fread(t, 1, 'uint32');
                cols = fread(t, 1, 'uint32');
                id = fread(t, 1, 'uint32');
                batchNum = fread(t, 1, 'uint32');
                nameSize = fread(t, 1, 'uint32');
                name = fread(t, nameSize, 'char');
                mat = fread(t, rows*cols, 'double');
                mat = (reshape(mat, rows, cols))';
                eval(sprintf('datas{batchNum}.%s = mat;', char(name)))
                %res = [res; mat];
                %image(myImgAx, mat);
                clear rows cols id nameSize name
%              case 'r'
%                 rows = fread(t, 1, 'uint32');
%                 cols = fread(t, 1, 'uint32');
%                 mat = fread(t, rows*cols, 'double');
%                 mat = (reshape(mat, rows, cols))';
%                 imgs{iter} = mat;
%                 iter = iter + 1;
%                 %img = [img; mat];
%                 %image(ax1, mat);
%                 %image(ax2, (reshape(inputBatch(length(res) , :) , [28 28]))');
%                 clear rows cols
            case 'p'
                msgSize = fread(t, 1, 'uint16');
                disp(char(rot90(fread(t, msgSize, 'char'))));
                clear msgSize
            otherwise
                disp(sprintf('%s is not a recognized header character\n', char(header)))
        end
        drawnow;
 end 
catch err
    rethrow(err)
end

if(batchNum >= N)
   display('finished reading');
    stop;
end




