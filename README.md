# Neural-Network-Engine

Summary: 
A personal project created as an exercise in understanding the basics and low-level operation of machine learning. 
Contains a self-written matrix library and several kinds of "layer" classes that when connected together, form a network. 
The NNE is currently geared towards solving the MNIST handwritten digit classification problem but is written such that it 
can be applied to more general problems in the future.

Using it: 
To use the NNE, one must have Matlab. The NNE is a win32 executable that communicates with Matlab via TCP sockets; 
Matlab sends training data and the win32 app sends results back to Matlab for plotting.
