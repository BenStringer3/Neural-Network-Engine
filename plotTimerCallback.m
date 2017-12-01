plotNow(1:batchNum, [arrayfun(@(a) a{1}.fitness, datas); movmean(arrayfun(@(a) a{1}.fitness, datas), 100); movmean(arrayfun(@(a) a{1}.fitness, datas), 1000)]);

serverResponse = false;
sendNNE(t,uint8('p')); 
% while ~serverResponse
%     readNNE;
% end
% serverResponse = false

%plotNow(fits(:,1), [fits(:,2), movmean(fits(:,2), 100), fits(:,3)]); 
somethingToPlot = false; 
for jj = batchNum:-1:1
    tmpCell = struct2cell(datas{jj});
    fields = fieldnames(datas{jj});
    for k = 1:length(fields) %for all the fields at this time step
        if isempty(strfind(fields{k}, 'pool_act')) %if a field doesn't have 'pool_act' in its name
            tmpCell{k} = []; %remove it from out temporary cell
        else %if a field DOES have 'pool_act' in its name
           somethingToPlot = true; %we'll mark this time step for plotting
        end
    end
    tmpCell(~cellfun('isempty',tmpCell)); %remove all the empty spots from our temporary cell
    if somethingToPlot 
        imgNow(tmpCell, (reshape(inputBatch(jj, :), [28 28]))', datas{jj}.results, jj);
        break;
    end

end
clear jj
