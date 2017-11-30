#pragma once
#include "Matrix.h"

enum LayerType {
	pooling,
	convolutional,
	mlp,
	elemWise
};

class Layer
{
public:
	Layer();
	~Layer();
	unsigned int lyrRows;
	unsigned int lyrCols;
	Matrix activations; //lyrRows x lyrCols
	Matrix dEdA; //lyrRows x lyrCols
	bool outputConnected;
	virtual void feedFwd() = 0;
	virtual void backProp() = 0;

	LayerType layerType;
	string name;
	unsigned int tmpChildIter;
	Layer * prevLyr;
protected:
	Matrix_pr prevLyrDels;
	Matrix_pr inputs;  //vector form
	
	unsigned int iteration;
	//Matrix dels;
	//Matrix_pr dEdA;

};

