#pragma once
#include "Layer.h"

enum PoolingType {
	Max,
	Avg,
};
class Pooling :
	public Layer
{
public:
	Pooling(PoolingType type);
	Pooling();
	~Pooling();
	void connect(Layer * prevLyr, unsigned int input2WinRatio, unsigned int prevLyr2ThisLyrRatio);
	void feedFwd();
	void backProp();
private:
	unsigned int padding;
	//only one receiving neuron 
	unsigned int winSideLen; //assumes only square windows for now
	unsigned int stride;
	PoolingType poolingType;
	Layer * prevLyr;
	Matrix zerosPadding;
};

