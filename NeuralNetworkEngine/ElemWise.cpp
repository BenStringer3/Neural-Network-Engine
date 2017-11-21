#include "stdafx.h"
#include "ElemWise.h"


ElemWise::ElemWise(ElemWiseType elemWiseType, double learningRate)
{
	this->elemWiseType = elemWiseType;


	this->learningRate = learningRate;

	this->gain = 1;
}


ElemWise::~ElemWise()
{
}

void ElemWise::connect(Layer * prevLyr)
{
	this->lyrRows = prevLyr->lyrRows;
	this->lyrCols = prevLyr->lyrCols;
	this->activations = Matrix(this->lyrRows, this->lyrCols);
	this->dEdA = Matrix(this->lyrRows, this->lyrCols);

	if (elemWiseType == Sig) {
		this->z = Matrix(this->lyrRows, this->lyrCols);
	}
	else if (elemWiseType == Bias) {
		this->bias = Matrix::random(this->lyrRows, this->lyrCols);
	}

	this->inputs = Matrix_pr(this->lyrRows, this->lyrCols);
	this->prevLyrDels = Matrix_pr(this->lyrRows, this->lyrCols);
	//this->inputs =  (prevLyr->activations.reshape(prevLyr->lyrRows * prevLyr->lyrCols, 1)).block(0, 0, prevLyr->lyrRows * prevLyr->lyrCols, 1);
	this->inputs.pointTo(prevLyr->activations);// = (prevLyr->activations.reshape(this->lyrRows, this->lyrCols)).block(0, 0, this->lyrRows, this->lyrCols);
	//this->prevLyrDels = (prevLyr->dEdA.reshape(prevLyr->lyrRows * prevLyr->lyrCols, 1)).block(0, 0, prevLyr->lyrRows * prevLyr->lyrCols, 1);
	//this->prevLyrDels = (prevLyr->dEdA.reshape(this->lyrRows, this->lyrCols)).block(0, 0, this->lyrRows, this->lyrCols);
	this->prevLyrDels.pointTo(prevLyr->dEdA);
	prevLyr->outputConnected = true;
}

void ElemWise::feedFwd()
{
	Matrix tmp(this->lyrRows, this->lyrCols);
	switch (elemWiseType) {
	case Gain:
		this->activations = inputs * gain;// .reshape(this->lyrRows, this->lyrCols) * gain;
		break;
	case Bias:
		tmp = (inputs + bias);
		//this->activations = (inputs + bias);// .reshape(this->lyrRows, this->lyrCols);
		for (int i = 0; i < this->lyrCols*this->lyrRows; i++) {
			this->activations.data[i] = (inputs + bias).data[i];
		}
		break;
	case Sig:
		this->z = inputs;// .reshape(this->lyrRows, this->lyrCols);
		//this->activations = (sig(&z));
		tmp = (sig(&z));
		for (int i = 0; i < this->lyrCols*this->lyrRows; i++) {
			this->activations.data[i] = (tmp).data[i];
		}
		break;
	}
	dEdA *= 0;
}

void ElemWise::backProp()
{
	switch (elemWiseType) {
	case Gain:
		this->prevLyrDels = Matrix::ones(this->lyrRows, this->lyrCols) * gain;
		break;
	case Bias:
		this->prevLyrDels = dEdA;// .reshape(this->lyrRows, this->lyrCols);
		this->bias -= dEdA;
		break;
	case Sig:
		this->prevLyrDels = dEdA.cwiseProduct(sig_prime(&z));// .reshape(this->lyrRows, this->lyrCols);
		break;
	}
}

void ElemWise::setGain(double gain) {
		this->gain = gain;
}

Matrix ElemWise::sig(Matrix *x) {
	Matrix ones = Matrix::ones(x->rows, x->cols);
	return ones.cwiseQuotient((ones + (-1.0 * (*x)).exp()));
}

Matrix ElemWise::sig_prime(Matrix *x) {
	Matrix tmp = sig(x);
	return tmp.cwiseProduct(Matrix::ones(x->rows, x->cols) - tmp);
}
