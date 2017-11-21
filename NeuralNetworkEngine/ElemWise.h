#pragma once
#include "Layer.h"

enum ElemWiseType {
	Gain,
	Bias,
	Sig
};

class ElemWise :
	public Layer
{
public:
	ElemWise(ElemWiseType elemWiseType, double learningRate);
	~ElemWise();
	void connect(Layer * prevLyr);
	void feedFwd();
	void backProp();
	void setGain(double gain);
private:
	ElemWiseType elemWiseType;

	
	//gain mode
	double gain;

	//bias mode
	Matrix bias;
	double learningRate;

	//sig mode
	Matrix z;
	Matrix sig(Matrix * x);
	Matrix sig_prime(Matrix * x);
};

