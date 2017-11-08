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
	~Pooling();
	void connect(Layer * prevLyr, unsigned int winSideLen, unsigned int stride);
	void feedFwd();
	void backProp();
private:
	//no padding
	//only one receiving neuron 
	unsigned int winSideLen; //assumes only square windows for now
	unsigned int stride;
	PoolingType poolingType;
	Layer * prevLyr;
};

