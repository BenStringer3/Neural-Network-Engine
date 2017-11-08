#pragma once
#include "Layer.h"
class Convolutional :
	public Layer
{
public:
	Convolutional();
	~Convolutional();
	void connect(Layer * prevLyr, unsigned int winSideLen, unsigned int stride);
	void feedFwd();
	void backProp();
private:
	unsigned int padding;
	//only one receiving neuron 
	unsigned int winSideLen; //assumes only square windows for now
	unsigned int stride;
	Matrix weights;
	Matrix weightFaults;
	Layer * prevLyr;
};

