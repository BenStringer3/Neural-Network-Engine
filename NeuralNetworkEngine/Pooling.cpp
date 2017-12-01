#include "stdafx.h"
#include "Pooling.h"
#include "cc.h"
#include <iomanip>


Pooling::Pooling(PoolingType type)
{
	poolingType = type;
	layerType = pooling;
	tmpChildIter = 0;
	velocity = 0;
}

Pooling::Pooling()
{
}

Pooling::~Pooling()
{
}

void Pooling::connect(Layer * prevLyr, unsigned int winSideLen, unsigned int prevLyr2ThisLyrRatio)
{
	unsigned int i = 0;
	this->prevLyr = prevLyr;

	this->winSideLen = winSideLen;
	this->stride = prevLyr2ThisLyrRatio;

	//padding = floor(winSideLen / 2);

	this->lyrRows = floor((prevLyr->lyrRows - winSideLen) / stride) + 1;
	this->lyrCols = floor((prevLyr->lyrCols - winSideLen) / stride) + 1;
	this->activations = Matrix(lyrRows, lyrCols);
	this->dEdA = Matrix(lyrRows, lyrCols);

	//zerosPadding = Matrix(prevLyr->lyrRows, prevLyr->lyrCols);
	inputs = Matrix_pr(prevLyr->lyrRows, prevLyr->lyrCols);
	prevLyrDels = Matrix_pr(prevLyr->lyrRows , prevLyr->lyrCols);
	inputs.pointTo(prevLyr->activations);
	prevLyrDels.pointTo(prevLyr->dEdA);
	//(inputs.block(padding , padding, prevLyr->lyrRows, prevLyr->lyrCols)).pointTo(prevLyr->activations);
	/*for (int i = padding; i < prevLyr->lyrRows + padding; i++) {
		for (int j = padding; j < prevLyr->lyrCols + padding; j++) {
			inputs.data[i*inputs.cols + j] = &prevLyr->activations.data[(i - padding)*prevLyr->lyrCols + j - padding];
			this->prevLyrDels.data[i*prevLyrDels.cols + j] = &prevLyr->dEdA.data[(i - padding)*prevLyr->lyrCols + j - padding];
		}
	}*/
}


void Pooling::calculateActivations()
{
	unsigned int i, j;
	for (i = 0; i < activations.rows; i++) {
		for (j = 0; j < activations.cols; j++) {
			switch (poolingType) {
			case Max:
				//cout << inputs.block(i*stride, j*stride, winSideLen, winSideLen) << endl;
				this->activations(i, j) = (inputs.block(i*stride, j*stride, winSideLen, winSideLen)).max();
				break;
			case Avg:
				this->activations(i, j) = (inputs.block(i*stride, j*stride, winSideLen, winSideLen)).avg();
				break;
			}
		}
	}

}


void Pooling::backProp()
{
	unsigned int i, j, r, c;
	//inputs *= 0;
	for (i = 0; i < activations.rows; i++) {
		for (j = 0; j < activations.cols; j++) {
			switch (poolingType) {
			case Max:
				
				(inputs.block(i*stride, j*stride, winSideLen, winSideLen)).maxLocation(r, c);
				(prevLyrDels.block(i*stride, j*stride, winSideLen, winSideLen))(r, c) += this->dEdA(i, j);
				
				break;
			case Avg:
				(prevLyrDels.block(i*stride, j*stride, winSideLen, winSideLen)) += this->dEdA(i, j) / lyrRows / lyrCols * Matrix::ones(winSideLen *  winSideLen, 1);
				//this->inputs += this->activations(i, j) / lyrRows / lyrCols * Matrix::ones(winSideLen*winSideLen, 1);
				break;
			}
		}
	}
	//this->zerosPadding *= 0;
}

