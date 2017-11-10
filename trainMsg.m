function msg = trainMsg(inputBatch, outputBatch)
    [batchSize, numInputs] = size(inputBatch);
    [m, numOutputs] = size(outputBatch);
    %TODO more dimensionality checks
    %if m ~= batchSize
    clear m
    %% turn the data into a tcp message


    %create the message header and such forth
    msg = [uint8('t'), swapbytes(typecast(uint32(batchSize), 'uint8'))];
    %put the inputBatch into the message vector (topleft to bottom right)
    for i=1:batchSize
        for ii=1:numInputs
            msg = [msg, swapbytes(typecast(double(inputBatch(i, ii)), 'uint8'))];
        end
    end

    %put the outputBatch into the message vector (topleft to bottom right)
    for i=1:batchSize
        for ii=1:numOutputs
            msg = [msg, swapbytes(typecast(double(outputBatch(i, ii)), 'uint8'))];
        end
    end
    clear batchSize numInputs numOutputs inputBatch outputBatch

    msg = typecast(msg, 'uint8');
    

end