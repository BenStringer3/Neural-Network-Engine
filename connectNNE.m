function tcpIp = connectNNE(outputBufSize)
%connect to the NNE server
    tcpIp = tcpip('localhost', 4012, 'NetworkRole', 'client', 'Timeout', 7);
    tcpIp.OutputBufferSize = outputBufSize;
    tcpIp.InputBufferSize = outputBufSize*3/2;
    fopen(tcpIp);
end