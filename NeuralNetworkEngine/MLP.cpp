#include "stdafx.h"
#include "MLP.h"
#include "cc.h"

MLP::MLP(unsigned int rows, unsigned int cols, double learningRate)
{
	this->lyrRows = rows;
	this->lyrCols = cols;

	this->learningRate = learningRate;

	this->activations = Matrix(rows, cols);
	this->dEdA = Matrix(rows, cols);
	this->biases = Matrix::random(rows, cols);
	dEdB = Matrix(rows, cols);
	inertial_dEdB = Matrix(rows, cols);
	this->numPrevLyrsNrns = 0;
	this->inputs = Matrix_pr(0,1 );
	this->prevLyrDels = Matrix_pr(0,1 );

	iter = 1;
	iterPerEpoch = ITER_PER_EPOCH;

	this->velocity_dEdW = 0;
	this->velocity_dEdB = 0;
}


MLP::~MLP()
{
}

void MLP::connect(Layer * prevLyr)
{
	this->numPrevLyrsNrns += prevLyr->lyrRows * prevLyr->lyrCols;
	this->weights = Matrix::random(this->lyrRows * this-> lyrCols, this->numPrevLyrsNrns);
	dEdW = Matrix(this->lyrRows * this->lyrCols, this->numPrevLyrsNrns);
	inertial_dEdW = Matrix(this->lyrRows * this->lyrCols, this->numPrevLyrsNrns);

	//this->correctionCoeffs = Matrix(weights.rows, weights.cols);
	//this->dEdW = Matrix(this->lyrRows * this->lyrCols, prevLyr->lyrRows * prevLyr->lyrCols);

	//this->inputs = (prevLyr->activations.reshape(this->numPrevLyrsNrns, 1)).block(0, 0, this->numPrevLyrsNrns, 1); //TODO: replace with pointsTo ??
	this->inputs =		Matrix_pr::vertcat(this->inputs,		(prevLyr->activations.reshape(prevLyr->lyrRows * prevLyr->lyrCols, 1)).block(0, 0,  prevLyr->lyrRows * prevLyr->lyrCols, 1));
	this->prevLyrDels = Matrix_pr::vertcat(this->prevLyrDels,	(prevLyr->dEdA.reshape(prevLyr->lyrRows * prevLyr->lyrCols, 1)).block(0, 0, prevLyr->lyrRows * prevLyr->lyrCols, 1));

	prevLyr->outputConnected = true;
}

void MLP::feedFwd() {
	this->z = (weights * inputs).reshape(lyrRows, lyrCols) + biases;
	this->activations = sig(&z);
	dEdA *= 0;
}

void MLP::backProp() {
	//Matrix dEdW(weights.rows, weights.cols);
	Matrix dels;

	//if (outputConnected) {
		//dels = (this->dEdA.reshape(this->lyrRows * this->lyrCols, 1)).cwiseProduct((sig_prime(&z)).reshape(this->lyrRows * this->lyrCols, 1));
		//dels = (this->dEdA).cwiseProduct((sig_prime(&z)));
		//cout << this->dEdA << endl;
		//cout << sig_prime(&z) << endl;
		//this->dEdA.reshape(this->lyrRows * this->lyrCols, 1) = this->dEdA.reshape(this->lyrRows * this->lyrCols, 1).cwiseProduct((sig_prime(&z)).reshape(this->lyrRows * this->lyrCols, 1));
		this->dEdA = this->dEdA.cwiseProduct(sig_prime(&z));
		//cout << this->dEdA << endl;

	//}
	//else {
		//dels = (this->dEdA.reshape(this->lyrRows * this->lyrCols, 1));
	//}

	//dEdW = dels * inputs.transpose();
	dEdW +=  (this->dEdA.reshape(this->lyrRows * this->lyrCols, 1)) * inputs.transpose() ;
	//this->prevLyrDels = weights.transpose() * dels;
	this->prevLyrDels = weights.transpose() * (this->dEdA.reshape(this->lyrRows * this->lyrCols, 1));
	//this->correctionCoeffs = 0.9*this->correctionCoeffs + learningRate*dEdW;
	 //this->correctionCoeffs;
	//biases -= dels.reshape(lyrRows, lyrCols);
	
	//biases -= this->dEdA;
	//weights -= learningRate*dEdW;
	dEdB += dEdA;

	if (iter % iterPerEpoch == 0) {
		inertial_dEdB = BETA1*inertial_dEdB + (1 - BETA1)*dEdB / iterPerEpoch;
		velocity_dEdB = BETA2*velocity_dEdB + (1 - BETA2)*((dEdB.cwiseProduct(dEdB)).sum() / iterPerEpoch);
		biases -= LEARNING_RATE / sqrt(velocity_dEdB + E)*(sqrt(1 - pow(BETA2, iter)) / (1 - pow(BETA1, iter))*inertial_dEdB);

		inertial_dEdW = BETA1*inertial_dEdW + (1 - BETA1)*dEdW / iterPerEpoch;
		velocity_dEdW = BETA2*velocity_dEdW + (1 - BETA2)*((dEdW.cwiseProduct(dEdW)).sum() / iterPerEpoch);
		weights -= LEARNING_RATE / sqrt(velocity_dEdW + E)*(sqrt(1 - pow(BETA2, iter)) / (1 - pow(BETA1, iter))*inertial_dEdW);

		//inertial_dEdB = (INERTIA * inertial_dEdB + learningRate*dEdB / iter);
		//biases -= inertial_dEdB;

		//inertial_dEdW = (INERTIA * inertial_dEdW + learningRate*dEdW / iter);
		//weights -= inertial_dEdW; ;// this->correctionCoeffs;
		//
		dEdW *= 0;
		dEdB *= 0;
		//iter = 0;
	}
	iter++;
}

Matrix MLP::sig(Matrix *x) {
	Matrix ones = Matrix::ones(x->rows, x->cols);
	return ones.cwiseQuotient((ones + (-1.0 * (*x)).exp()));
}

Matrix MLP::sig_prime(Matrix *x) {
	Matrix tmp = sig(x);
	return tmp.cwiseProduct(Matrix::ones(x->rows, x->cols) - tmp);
}

void MLP::setLearningRate(double lr)
{
	this->learningRate = lr;
}