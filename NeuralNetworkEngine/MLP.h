#pragma once
#include "Layer.h"
class MLP :
	public Layer
{
public:
	MLP(unsigned int rows, unsigned int cols, double learningRate);
	~MLP();
	void connect(Layer * prevLyr);
	//void feedFwd();
	virtual void backProp();
	void setLearningRate(double lr);

private:
	double learningRate;
	unsigned int numPrevLyrsNrns;
	Matrix weights;
	//Matrix biases; // vector form
	//Matrix z; //vector form
	//Matrix sig(Matrix * x);
	//Matrix sig_prime(Matrix * x);
	//Matrix dEdW;
	//Matrix dEdB;
	//Matrix inertial_dEdW;
	//Matrix inertial_dEdB;
	//double velocity_dEdW;
	//double velocity_dEdB;
	//unsigned int iterPerEpoch;
	//unsigned int iter;

	virtual void calculateActivations();
};

