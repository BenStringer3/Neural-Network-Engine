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
	LayerType layerType;
	unsigned int lyrRows;
	unsigned int lyrCols;
	Matrix activations; //lyrRows x lyrCols
	Matrix dEdA; //lyrRows x lyrCols
	//Matrix dEdA_accum; //lyrRows x lyrCols
	//bool outputConnected;
	void feedFwd();// = 0;
	virtual void backProp() = 0;
	void applyCorrections(Matrix & thingToBeCorrected);

	
	string name;
	unsigned int tmpChildIter;
	Layer * prevLyr;
protected:
	Matrix_pr prevLyrDels;
	Matrix_pr inputs;  //vector form
	virtual void calculateActivations() = 0;
	//virtual void calculatePrevLyrdEdA() = 0;
	unsigned int iteration;
	//Matrix dels;
	//Matrix_pr dEdA;
	unsigned int iterPerEpoch;
	unsigned int iter;

	Matrix inertia;
	double velocity;
	Matrix correctionToBeAccumulated;

};

