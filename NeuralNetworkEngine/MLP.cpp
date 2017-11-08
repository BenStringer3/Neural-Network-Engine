#include "stdafx.h"
#include "MLP.h"


MLP::MLP(unsigned int rows, unsigned int cols)
{
	this->lyrRows = rows;
	this->lyrCols = cols;

	this->activations = Matrix(rows, cols);
	this->biases = Matrix::random(rows, cols);
	//this->biasFaults = Matrix(rows, cols);
}


MLP::~MLP()
{
}

void MLP::connect(Layer * prevLyr)
{

	this->weights = Matrix::random(this->lyrRows * this-> lyrCols, prevLyr->lyrRows * prevLyr->lyrCols);
	//this->weightFaults = Matrix(this->lyrRows * this->lyrCols, prevLyr->lyrRows * prevLyr->lyrCols);

	this->inputs = (prevLyr->activations.reshape(prevLyr->lyrRows * prevLyr->lyrCols, 1)).block(0, 0, prevLyr->lyrRows * prevLyr->lyrCols, 1);
	prevLyr->outputConnected = true;
}

void MLP::feedFwd() {
	this->z = weights * inputs + biases;
	this->activations = sig(&z).reshape(this->lyrRows, this->lyrCols);
}

void MLP::backProp() {
	Matrix weightFaults;
	Matrix dels;

	if (outputConnected) {
		dels = (this->activations.reshape(this->lyrRows * this->lyrCols, 1)).cwiseProduct(sig_prime(&z));
	}
	else {
		dels = (this->activations.reshape(this->lyrRows * this->lyrCols, 1));
	}

	weightFaults = dels * inputs.transpose();
	this->inputs = weights.transpose() * dels;

	weights -= weightFaults;
	biases -= dels;
}

Matrix MLP::sig(Matrix *x) {
	Matrix ones = Matrix::ones(x->cols*x->rows, 1);
	return ones.cwiseQuotient((ones + (-1.0 * (*x)).exp()));
}

Matrix MLP::sig_prime(Matrix *x) {
	Matrix tmp = sig(x);
	return tmp.cwiseProduct(Matrix::ones(x->rows, 1) - tmp);
}