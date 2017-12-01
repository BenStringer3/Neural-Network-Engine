#include "stdafx.h"
#include "ElemWise.h"
#include "cc.h"

ElemWise::ElemWise(ElemWiseType elemWiseType, double learningRate)
{
	this->elemWiseType = elemWiseType;


	this->learningRate = learningRate;

	//this->gain = 1;
	layerType = elemWise;
	velocity = 0;
}


ElemWise::~ElemWise()
{
}

void ElemWise::connect(Layer * prevLyr)
{
	this->prevLyr = prevLyr;
	this->lyrRows = prevLyr->lyrRows;
	this->lyrCols = prevLyr->lyrCols;
	this->activations = Matrix(this->lyrRows, this->lyrCols);
	this->dEdA = Matrix(this->lyrRows, this->lyrCols);

	if (elemWiseType == Sig) {
		//this->z = Matrix(this->lyrRows, this->lyrCols);
	}
	else if (elemWiseType == Bias) {
		INITIAL_BIASES
		//this->biases = Matrix::random(this->lyrRows, this->lyrCols);
		this->correctionToBeAccumulated = Matrix(biases.rows, biases.cols);
		this->inertia = Matrix(biases.rows, biases.cols);
	}

	this->inputs = Matrix_pr(this->lyrRows, this->lyrCols);
	this->prevLyrDels = Matrix_pr(this->lyrRows, this->lyrCols);
	//this->inputs =  (prevLyr->activations.reshape(prevLyr->lyrRows * prevLyr->lyrCols, 1)).block(0, 0, prevLyr->lyrRows * prevLyr->lyrCols, 1);
	this->inputs.pointTo(prevLyr->activations);// = (prevLyr->activations.reshape(this->lyrRows, this->lyrCols)).block(0, 0, this->lyrRows, this->lyrCols);
	//this->prevLyrDels = (prevLyr->dEdA.reshape(prevLyr->lyrRows * prevLyr->lyrCols, 1)).block(0, 0, prevLyr->lyrRows * prevLyr->lyrCols, 1);
	//this->prevLyrDels = (prevLyr->dEdA.reshape(this->lyrRows, this->lyrCols)).block(0, 0, this->lyrRows, this->lyrCols);
	this->prevLyrDels.pointTo(prevLyr->dEdA);
	//prevLyr->outputConnected = true;
}

void ElemWise::calculateActivations()
{
	//Matrix tmp(this->lyrRows, this->lyrCols);
	switch (elemWiseType) {
	case Bias:
		this->activations = (inputs + biases);
		//this->activations = (inputs + bias);// .reshape(this->lyrRows, this->lyrCols);
		/*for (int i = 0; i < this->lyrCols*this->lyrRows; i++) {
			this->activations.data[i] = (inputs + biases).data[i];
		}*/
		break;
	case Sig:
		activations = sig(inputs);
		//this->z = inputs;// .reshape(this->lyrRows, this->lyrCols);
		////this->activations = (sig(&z));
		//tmp = (sig(&z));
		//for (int i = 0; i < this->lyrCols*this->lyrRows; i++) {
		//	this->activations.data[i] = (tmp).data[i];
		//}
		break;
	}
}

void ElemWise::backProp()
{
	switch (elemWiseType) {
	case Bias:
		this->prevLyrDels = dEdA;// .reshape(this->lyrRows, this->lyrCols);
		correctionToBeAccumulated += dEdA;
		applyCorrections(biases);
		break;
	case Sig:
		this->prevLyrDels = dEdA.cwiseProduct(activations.cwiseProduct(Matrix::ones(activations.rows, activations.cols) - activations));// .reshape(this->lyrRows, this->lyrCols);
		break;
	}
}



Matrix ElemWise::sig(const Matrix_pr &x) {
	Matrix ones = Matrix::ones(x.rows, x.cols);
	return ones.cwiseQuotient((ones + (x*-1.0).exp()));
}

//Matrix ElemWise::sig_prime() {
//	//Matrix tmp = sig(x);
//	return activations.cwiseProduct(Matrix::ones(activations.rows, activations.cols) - activations);
//}


