#pragma once
#include "Layer.h"
class MLP :
	public Layer
{
public:
	MLP(unsigned int rows, unsigned int cols);
	~MLP();
	void connect(Layer * prevLyr);
	void feedFwd();
	void backProp();
private:
	Matrix weights;
	Matrix biases; // vector form
	Matrix z; //vector form
	Matrix sig(Matrix * x);
	Matrix sig_prime(Matrix * x);
};

