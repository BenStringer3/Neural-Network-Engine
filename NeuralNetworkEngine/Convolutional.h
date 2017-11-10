#pragma once
#include "Layer.h"
class Convolutional :
	public Layer
{
public:
	Convolutional();
	~Convolutional();
	void connect(Layer * prevLyr, unsigned int input2WinRatio, unsigned int prevLyr2ThisLyrRatio);
	void feedFwd();
	void backProp();
	void setWeights(const Matrix & mat);
private:
	unsigned int padding;
	//only one receiving neuron 
	unsigned int winSideLen; //assumes only square windows for now
	unsigned int stride;
	Matrix weights;
	Matrix weightFaults;
	Layer * prevLyr;
};

