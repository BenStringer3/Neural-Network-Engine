#include "stdafx.h"
#include "Convolutional.h"


Convolutional::Convolutional()
{

}


Convolutional::~Convolutional()
{
}

void Convolutional::connect(Layer * prevLyr, unsigned int winSideLen, unsigned int stride)
{
	unsigned int i = 0, m = 1, n = 1;
	if (winSideLen > prevLyr->lyrRows || winSideLen > prevLyr->lyrCols) {
		throw std::out_of_range("window larger than previous layer");
	}

	this->prevLyr = prevLyr;
	this->winSideLen = winSideLen;
	this->stride = stride;

	padding = floor(winSideLen / 2);

	this->weights = Matrix::random(1, winSideLen*winSideLen);
	this->weightFaults = Matrix(1, winSideLen*winSideLen);

	 do {
		i = (m - 1)*stride + winSideLen  ;
		m++;
	} while (i < prevLyr->lyrRows + padding*2);
	do {
		i = (n - 1)*stride + winSideLen;
		n++;
	} while (i < prevLyr->lyrCols + padding * 2);

	this->activations = Matrix((prevLyr->lyrRows + padding*2 - winSideLen)/stride, (prevLyr->lyrCols + padding * 2 - winSideLen) / stride);

	//this->inputs = prevLyr->activations.block(0, 0, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
}


void Convolutional::backProp()
{
	Matrix weightFaults(weights.rows, weights.cols);
	Matrix inputz(winSideLen * winSideLen, 1);
	unsigned int a, b, c, d, f, g;
	Matrix e;

	for (int i = 0; i < activations.rows ; i++) {
		for (int j = 0; j < activations.cols; j++) {
			inputz = Matrix(winSideLen , winSideLen);
			c = winSideLen;
			d = winSideLen;
			if (i*stride < padding) {
				a = padding - i*stride;
				c -= a;
				f = 0;
			}
			else {
				a = 0;
				f = (i - padding)*stride;
			}
			if (j*stride < padding) {
				b = padding - j*stride;
				d -= b;
				g = 0;
			}
			else {
				b = 0;
				g = (j - padding)*stride;
			}
			if (i*stride + winSideLen > prevLyr->lyrRows + padding) {
				c -= (i*stride + winSideLen - prevLyr->lyrRows + padding);
			}
			if (j*stride + winSideLen > prevLyr->lyrRows + padding) {
				d -= (j*stride + winSideLen - prevLyr->lyrRows + padding);
			}
			e = Matrix(c, d);
			e = prevLyr->activations.block(f,g, c, d);
			inputz.block(a, b, c, d) = e;
			cout << inputz << endl;
			this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
			weightFaults += inputs.transpose() * (activations(i, j)) ;
			//this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
			//this->inputs += weights.transpose()* activations.reshape(winSideLen*winSideLen, 1);
		}
	}
	for (int i = 0; i < activations.rows; i++) {
		for (int j = 0; j < activations.cols; j++) {
			this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
			this->inputs += weights.transpose() * (activations(i, j));
		}
	}
	weights -= weightFaults;
}

void Convolutional::feedFwd()
{
	Matrix_pr a;
	for (int i = 0; i < activations.rows; i++) {
		for (int j = 0; j < activations.cols; j++) {
			a = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
			this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1); 
			this->activations(i, j) += (weights * inputs).data[0];
		}
	}
}
