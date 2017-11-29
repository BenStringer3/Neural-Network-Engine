#pragma once
#include "Matrix.h"

class Layer
{
public:
	Layer();
	~Layer();
	unsigned int lyrRows;
	unsigned int lyrCols;
	Matrix activations; //matrix form
	Matrix dEdA;
	bool outputConnected;
	virtual void feedFwd() = 0;
	virtual void backProp() = 0;
protected:
	Matrix_pr prevLyrDels;
	Matrix_pr inputs;  //vector form

	unsigned int iteration;
	//Matrix dels;
	//Matrix_pr dEdA;

};

