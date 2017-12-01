#include "stdafx.h"
#include "Convolutional.h"
#include "cc.h"

#define FLIP 0

Convolutional::Convolutional(double learningRate)
{
	this->learningRate = learningRate;
	iter = 1;
	iterPerEpoch = ITER_PER_EPOCH;
	this->velocity = 0;
	layerType = convolutional;
	tmpChildIter = 0;
}

Convolutional::Convolutional()
{
}


Convolutional::~Convolutional()
{
}

void Convolutional::connect(Layer * prevLyr, unsigned int winSideLen, unsigned int prevLyr2ThisLyrRatio)
{
	unsigned int i = 0;

	this->prevLyr = prevLyr;

	this->winSideLen = winSideLen;
	this->stride = prevLyr2ThisLyrRatio;

	padding = (unsigned int)floor(winSideLen / 2);

	this->weights = Matrix::random(1, winSideLen*winSideLen);
	this->correctionToBeAccumulated = Matrix(weights.rows, weights.cols);
	this->inertia = Matrix(weights.rows, weights.cols);

	this->lyrRows = (unsigned int)floor((prevLyr->lyrRows + padding * 2 - winSideLen) / stride) + 1 ;
	this->lyrCols = (unsigned int)floor((prevLyr->lyrCols + padding * 2 - winSideLen) / stride) + 1;
	this->activations = Matrix(lyrRows, lyrCols);

	this->dEdA = Matrix(lyrRows, lyrCols);
	
	zerosPadding = Matrix(prevLyr->lyrRows + 2 * padding, prevLyr->lyrCols + 2 * padding);
	inputs = Matrix_pr(prevLyr->lyrRows + 2 * padding, prevLyr->lyrCols + 2 * padding);
	prevLyrDels = Matrix_pr(prevLyr->lyrRows + 2 * padding, prevLyr->lyrCols + 2 * padding);
	inputs.pointTo(zerosPadding);
	prevLyrDels.pointTo(zerosPadding);
	//(inputs.block(padding , padding , prevLyr->lyrRows, prevLyr->lyrCols)).pointTo(prevLyr->activations);
	for (int i = padding; i < prevLyr->lyrRows + padding; i++) {
		for (int j = padding; j < prevLyr->lyrCols + padding; j++) {
			inputs.data[i*inputs.cols + j] = &prevLyr->activations.data[(i - padding)*prevLyr->lyrCols + j - padding];
			prevLyrDels.data[i*inputs.cols + j] = &prevLyr->dEdA.data[(i - padding)*prevLyr->lyrCols + j - padding];
		}
	}
}

//void Convolutional::backProp()
//{
//	unsigned int i, j;
//	//dEdW *= 0;
//
//	for (i = 0; i < activations.rows; i++) {
//		for (j = 0; j < activations.cols; j++) {
//			//cout << endl;
//			//for (int ii = 0; ii < inputs.rows; ii++) {
//			//for (int jj = 0; jj < inputs.rows; jj++) {
//			//if ((jj == j*stride || jj == j*stride + winSideLen) && (i*stride <= ii) && (ii < (i*stride + winSideLen))) {
//			//cout << "|";
//			//}
//
//			//cout << std::setw(5) << std::fixed << std::setprecision(2) << inputs(ii, jj) << " ";
//
//
//			//}
//			//cout << endl;
//			//}
//			//cout << dEdA(i, j) << endl;
//			//cout << weights << endl;
//			dEdW += (inputs.block(i*stride, j*stride, winSideLen, winSideLen)).reshape(1, winSideLen* winSideLen) * (dEdA(i, j));
//			//cout << dEdW << endl;
//			(prevLyrDels.block(i*stride, j*stride, winSideLen, winSideLen)).reshape(winSideLen * winSideLen, 1) += weights.transpose() * (dEdA(i, j));
//			zerosPadding *= 0;
//		}
//	}
//
//	//this->correctionCoeffs = 0.9*this->correctionCoeffs + learningRate*dEdW;
//	dEdB += dEdA;
//	
//	if (iter % iterPerEpoch == 0) {
//		inertial_dEdB = BETA1*inertial_dEdB + (1 - BETA1)*dEdB / iterPerEpoch;
//		velocity_dEdB = BETA2*velocity_dEdB + (1 - BETA2)*((dEdB.cwiseProduct(dEdB)).sum() / iterPerEpoch);
//		biases -=   LEARNING_RATE/sqrt(velocity_dEdB + E)*(sqrt(1-pow(BETA2, iter)) / (1 - pow(BETA1, iter))*inertial_dEdB);
//
//		inertial_dEdW = BETA1*inertial_dEdW + (1 - BETA1)*dEdW / iterPerEpoch;
//		velocity_dEdW = BETA2*velocity_dEdW + (1 - BETA2)*((dEdW.cwiseProduct(dEdW)).sum() / iterPerEpoch);
//		weights -= LEARNING_RATE / sqrt(velocity_dEdW + E)*(sqrt(1 - pow(BETA2, iter)) / (1 - pow(BETA1, iter))*inertial_dEdW);
//
//		//inertial_dEdB = (INERTIA * inertial_dEdB + learningRate*dEdB / iter);
//		//biases -= inertial_dEdB;
//
//		//inertial_dEdW = (INERTIA * inertial_dEdW + learningRate*dEdW / iter);
//		//weights -= inertial_dEdW; ;// this->correctionCoeffs;
//		//
//		dEdW *= 0;
//		dEdB *= 0;
//		//iter = 0;
//	}
//	iter++;
//}
void Convolutional::backProp() {
	unsigned int i, j, ii, jj;

	for (i = 0; i < activations.rows; i++) {
		for (j = 0; j < activations.cols; j++) {
			correctionToBeAccumulated += (inputs.block(i*stride, j*stride, winSideLen, winSideLen)).reshape(1, winSideLen* winSideLen) * (dEdA(i, j));
			(prevLyrDels.block(i*stride, j*stride, winSideLen, winSideLen)).reshape(winSideLen * winSideLen, 1) += weights.transpose() * (dEdA(i, j));
			//zerosPadding *= 0;
			//cout << zerosPadding << endl;
			if (i*stride < padding) {
				//zerosPadding *= 0;
				zerosPadding.block(0, 0, padding, zerosPadding.cols) *= 0; //top
			}
			if (j*stride < padding) {
				//zerosPadding *= 0;
				zerosPadding.block(0, 0, zerosPadding.rows, padding) *= 0; //left
			}
			if (j*stride + winSideLen >= zerosPadding.cols) {
				//zerosPadding *= 0;
				zerosPadding.block(0, zerosPadding.cols - padding, zerosPadding.rows, padding) *= 0; //right
			}
			if (i*stride + winSideLen >= zerosPadding.rows) {
				//zerosPadding *= 0;
				zerosPadding.block(zerosPadding.rows - padding, 0, padding, zerosPadding.cols) *= 0; //bottom
			}
			//cout << zerosPadding << endl;
		}
	}

	applyCorrections(weights);
}

void Convolutional::calculateActivations() {
		unsigned int i, j;
		for (i = 0; i < activations.rows; i++) {
			for (j = 0; j < activations.cols; j++) {
				this->activations(i, j) = (weights * (inputs.block(i*stride, j*stride, winSideLen, winSideLen)).reshape(winSideLen* winSideLen, 1)).data[0];// +biases(i, j);
			}
		}
}



//void Convolutional::feedFwd() {
//	unsigned int i, j;
//	for (i = 0; i < activations.rows; i++) {
//		for (j = 0; j < activations.cols; j++) {
//			//window.block(blokWin_i, blokWin_j, blokWin_height, blokWin_width).assign(prevLyr->activations.block(in_i, in_j, blokWin_height, blokWin_width));
//
//			//window.reshape(winSideLen* winSideLen, 1);
//			//cout << inputs.block(i*stride, j*stride, winSideLen, winSideLen) << endl;
//
//			/*cout << endl;
//			for (int ii = 0; ii < inputs.rows; ii++) {
//				for (int jj = 0; jj < inputs.rows; jj++) {
//					if ((jj == j*stride || jj == j*stride + winSideLen) && (i*stride <= ii) && (ii < (i*stride + winSideLen))) {
//						cout << "|";
//					}
//
//						cout << std::setw(5) << std::fixed << std::setprecision(2) << inputs(ii, jj) << " ";
//
//
//				}
//				cout << endl;
//			}*/
//			//cout << weights << endl;
//			this->activations(i, j) = (weights * (inputs.block(i*stride, j*stride, winSideLen, winSideLen)).reshape(winSideLen* winSideLen, 1)).data[0] +biases(i, j);
//			//cout << this->activations << endl;
//		}
//	}
//	dEdA *= 0;
//	//NNE_helper.printMat(activations);
//	//inputs *= 0;
//}
//#endif


void Convolutional::setWeights(const Matrix & mat)
{
	this->weights = mat;
}

void Convolutional::setLearningRate(double lr)
{
	this->learningRate = lr;
}
