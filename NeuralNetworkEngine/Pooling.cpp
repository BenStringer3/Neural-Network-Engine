#include "stdafx.h"
#include "Pooling.h"




Pooling::Pooling(PoolingType type)
{
	poolingType = type;
}

Pooling::~Pooling()
{
}

void Pooling::connect(Layer * prevLyr, unsigned int winSideLen, unsigned int stride)
{
	unsigned int i = 0, m = 1, n = 1;
	if (winSideLen > prevLyr->lyrRows || winSideLen > prevLyr->lyrCols) {
		throw std::out_of_range("window larger than previous layer");
	}

	this->prevLyr = prevLyr;
	this->winSideLen = winSideLen;
	this->stride = stride;

	do {
		i = (m - 1)*stride + winSideLen;
		m++;
	} while (i < prevLyr->lyrRows);
	do {
		i = (n - 1)*stride + winSideLen;
		n++;
	} while (i < prevLyr->lyrCols);

	this->activations = Matrix(m - 1, n - 1);

}

void Pooling::feedFwd()
{
	for (int i = 0; i < activations.rows; i++) {
		for (int j = 0; j < activations.cols; j++) {
			this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
			switch (poolingType) {
			case Max:
				this->activations(i, j) += inputs.max();
				break;
			case Avg:
				this->activations(i, j) += inputs.avg();
				break;
			}
		}
	}
}

void Pooling::backProp()
{
	unsigned int r, c;
	for (int i = 0; i < activations.rows; i++) {
		for (int j = 0; j < activations.cols; j++) {
			this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
			switch (poolingType) {
			case Max:
				this->inputs.maxLocation(r, c);
				this->inputs(r, c) += this->activations(i, j);
				break;
			case Avg:
				this->inputs += this->activations(i, j) / lyrRows / lyrCols * Matrix::ones(winSideLen*winSideLen, 1);
				break;
			}
		}
	}
}


