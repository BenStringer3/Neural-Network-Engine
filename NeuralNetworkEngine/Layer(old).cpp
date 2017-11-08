#include "stdafx.h"
#include "Layer.h"


Layer::Layer(unsigned int numNrns)
{

	j = numNrns;
	dels =	Matrix(j, 1); // j x 1
	acts =	Matrix(j, 1); // j x 1
	z =		Matrix(j, 1); // j x 1
	biases =Matrix::random(j, 1); // j x 1
	biasFaults = Matrix(j, 1); // j x 1
}

Layer::Layer(unsigned int rows, unsigned int cols)
{

	this->rows = rows;
	this->cols = cols;

	winRows = rows;
	winCols = cols;
	winStartRow = 0;
	winStartCol = 0;

	this->j = rows * cols;
	dels = Matrix(j, 1); // j x 1
	acts = Matrix(j, 1); // j x 1
	z = Matrix(j, 1); // j x 1
	biases = Matrix::random(j, 1); // j x 1
	biasFaults = Matrix(j, 1); // j x 1
}


Layer::~Layer()
{
}

void Layer::connect(Layer *postLyr) {
	postLyr->inputs = this->acts.block(0, 0, this->acts.rows, this->acts.cols);
	postLyr->weights = Matrix::random(postLyr->j, this->j);
	postLyr->weightFaults = Matrix(postLyr->j, this->j);

	this->postLyrWeights = &postLyr->weights;
	this->postLyrDels = &postLyr->dels;


}

void Layer::connect(Layer *postLyr,
	unsigned int inStartRow, unsigned int inStartCol, unsigned int inHeight, unsigned int inWidth,
	unsigned int outStartRow, unsigned int outStartCol, unsigned int outHeight, unsigned int outWidth) {
	postLyr->inputs = (*((*(this->acts.reshape(this->rows, this->cols))).block(outStartRow, outStartCol, outHeight, outWidth)).reshape(outHeight * outWidth, 1));
	postLyr->weights = Matrix::random(inHeight * inWidth, outHeight * outWidth);
	postLyr->weightFaults = Matrix(inHeight * inWidth, outHeight * outWidth);

	this->postLyrWeights = &postLyr->weights;
	this->postLyrDels = &postLyr->dels;

	postLyr->winRows =		inHeight;
	postLyr->winCols =		inWidth;
	postLyr->winStartRow =	inStartRow;
	postLyr->winStartCol =	inStartCol;
}

void Layer::feedFwd()
{
	Matrix tmp;
	z.block(winStartRow, winStartCol, winRows, winCols) = weights*(inputs);// +biases;
	tmp = z + biases;
	acts = sig(&tmp);
}

void Layer::backProp()
{
		dels = ((*postLyrWeights).transpose() * (*postLyrDels)).cwiseProduct(sig_prime(&z));
		//nx1 = [j x k] [k x 1] .* [j x 1]
}

void Layer::accum()
{
	weightFaults += dels * (inputs).transpose();
	//j x i = [j x 1] [1 x i]
	biasFaults += dels;
}

void Layer::correct()
{
	weights -= weightFaults;
	weightFaults -= weightFaults;
	biases -= biasFaults;
	biasFaults -= biasFaults;
}

void Layer::shift(int upDown, int leftRight)
{
	winStartRow += upDown;
	winStartCol += leftRight;
}

Matrix Layer::sig(Matrix *x) {
	Matrix ones = Matrix::ones(x->cols*x->rows, 1);
	return ones.cwiseQuotient((ones + (-1.0 * (*x)).exp()));
}

Matrix Layer::sig_prime(Matrix *x) {
	Matrix tmp = sig(x);
	return tmp.cwiseProduct(Matrix::ones(x->rows, 1) - tmp);
}


