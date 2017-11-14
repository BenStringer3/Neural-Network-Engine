#include "stdafx.h"
#include "MLP.h"


MLP::MLP(unsigned int rows, unsigned int cols)
{
	this->lyrRows = rows;
	this->lyrCols = cols;

	this->activations = Matrix(rows, cols);
	this->biases = Matrix::random(rows, cols);
	this->numPrevLyrsNrns = 0;
	this->inputs = Matrix_pr(0, 1);
}


MLP::~MLP()
{
}

void MLP::connect(Layer * prevLyr)
{
	this->numPrevLyrsNrns += prevLyr->lyrRows * prevLyr->lyrCols;
	this->weights = Matrix::random(this->lyrRows * this-> lyrCols, this->numPrevLyrsNrns);
	//this->weightFaults = Matrix(this->lyrRows * this->lyrCols, prevLyr->lyrRows * prevLyr->lyrCols);

	//this->inputs = (prevLyr->activations.reshape(this->numPrevLyrsNrns, 1)).block(0, 0, this->numPrevLyrsNrns, 1); //TODO: replace with pointsTo ??
	this->inputs = Matrix_pr::vertcat(this->inputs, (prevLyr->activations.reshape(prevLyr->lyrRows * prevLyr->lyrCols, 1)).block(0, 0, prevLyr->lyrRows * prevLyr->lyrCols, 1));
	prevLyr->outputConnected = true;
}

void MLP::feedFwd() {
	this->z = (weights * inputs).reshape(lyrRows, lyrCols) + biases;
	this->activations = sig(&z).reshape(this->lyrRows, this->lyrCols);
}

void MLP::backProp() {
	Matrix weightFaults;
	Matrix dels;

	if (outputConnected) {
		dels = (this->activations.reshape(this->lyrRows * this->lyrCols, 1)).cwiseProduct((sig_prime(&z)).reshape(this->lyrRows * this->lyrCols, 1));
	}
	else {
		dels = (this->activations.reshape(this->lyrRows * this->lyrCols, 1));
	}

	weightFaults = dels * inputs.transpose();
	this->inputs = weights.transpose() * dels;

	weights -= weightFaults;
	biases -= dels.reshape(lyrRows, lyrCols);
}

Matrix MLP::sig(Matrix *x) {
	Matrix ones = Matrix::ones(x->rows, x->cols);
	Matrix a, b, c, d; 
	cout << *x << endl;
	a = (-1.0 * (*x));
	b = a.exp();
	c = ones + b;
	d = ones.cwiseQuotient(c);
	return ones.cwiseQuotient((ones + (-1.0 * (*x)).exp()));
}

Matrix MLP::sig_prime(Matrix *x) {
	Matrix tmp = sig(x);
	return tmp.cwiseProduct(Matrix::ones(x->rows, x->cols) - tmp);
}