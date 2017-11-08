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
	bool outputConnected;

protected:
	
	Matrix_pr inputs;  //vector form
	//Matrix dels;
	//Matrix_pr postLyrDels;
	virtual void feedFwd() = 0;
	virtual void backProp() = 0;
};

