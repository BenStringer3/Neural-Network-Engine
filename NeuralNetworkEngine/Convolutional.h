#pragma once
#include "Layer.h"
class Convolutional :
	public Layer
{
public:
	Convolutional(double learningRate);
	Convolutional();
	~Convolutional();
	void connect(Layer * prevLyr, unsigned int input2WinRatio, unsigned int prevLyr2ThisLyrRatio);
	//void feedFwd();
	virtual void backProp();
	void setWeights(const Matrix & mat);
	void setLearningRate(double lr);

private:
	unsigned int padding;
	//only one receiving neuron 
	unsigned int winSideLen; //assumes only square windows for now
	unsigned int stride;
	Matrix weights;
	//Matrix biases;
	//Matrix dEdW;
	//Layer * prevLyr;
	Matrix zerosPadding;
	double learningRate;
	//Matrix dEdW;
	//Matrix dEdB;
	//Matrix inertial_dEdW;
	//Matrix inertial_dEdB;
	//double velocity_dEdW;
	//double velocity_dEdB;
	/*unsigned int iterPerEpoch;
	unsigned int iter;*/

	virtual void calculateActivations();
	//virtual void calculatePrevLyrdEdA();
};

