#pragma once
#include "Layer.h"

enum ElemWiseType {
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
	//void feedFwd();
	virtual void backProp();
private:
	ElemWiseType elemWiseType;


	//bias mode
	Matrix biases;
	double learningRate;

	//sig mode
	//Matrix z;
	Matrix sig(const Matrix_pr &x);
	//Matrix sig_prime();

	virtual void calculateActivations();

};

