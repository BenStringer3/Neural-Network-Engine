function sendNNE(tcpIp, msg)
tcpIp.ByteOrder = 'bigEndian';
     try
%         if expectResponse
%             fwrite(tcpIp,msg(1))
%             tcpIp.ByteOrder = 'littleEndian';
%             response = fread(tcpIp, 1, 'char');
%             if strcmp(char(response), 'o')
%                 tcpIp.ByteOrder = 'bigEndian';
%                 fwrite(tcpIp,msg(2:end))
%             elseif (strcmp(char(response), 'e'))
%                 disp('NNE rejected your request\n')
%             else
%                 disp(sprintf('%s is not a recognized confirmation character character\n', char(response)))
%             end
%         else
            fwrite(tcpIp,msg)
%         end
    catch err
        fclose(tcpIp);
        rethrow(err)
    end

end