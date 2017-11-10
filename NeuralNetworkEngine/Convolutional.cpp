#include "stdafx.h"
#include "Convolutional.h"


Convolutional::Convolutional()
{

}


Convolutional::~Convolutional()
{
}

void Convolutional::connect(Layer * prevLyr, unsigned int input2WinRatio, unsigned int prevLyr2ThisLyrRatio)
{
	unsigned int i = 0;


	this->prevLyr = prevLyr;
	if (prevLyr->lyrRows < prevLyr->lyrCols) { 
		this->winSideLen = prevLyr->lyrRows / input2WinRatio;
	}
	else {
		this->winSideLen = prevLyr->lyrCols / input2WinRatio;
	}
	this->stride = prevLyr2ThisLyrRatio;

	padding = floor(winSideLen / 2);

	this->weights = Matrix::random(1, winSideLen*winSideLen);
	this->weightFaults = Matrix(1, winSideLen*winSideLen);


	this->lyrRows = floor((prevLyr->lyrRows + padding * 2 - winSideLen + 1) / stride) + 1 ;
	this->lyrCols = floor((prevLyr->lyrCols + padding * 2 - winSideLen + 1) / stride) + 1;
	this->activations = Matrix(lyrRows, lyrCols);
	//this->activations = Matrix((prevLyr->lyrRows  - winSideLen) / stride , (prevLyr->lyrCols  - winSideLen) / stride);
	
}

void Convolutional::backProp()
{
	Matrix weightFaults(weights.rows, weights.cols);
	int i, j;
	int in_i, in_j, win_i, win_j, blokWin_i, blokWin_j, blokWin_height, blokWin_width, in_height, in_width;
	Matrix window(winSideLen, winSideLen);
	Matrix tmp;

	in_height = prevLyr->lyrRows;
	in_width = prevLyr->lyrCols;
	for (i = 0; i < activations.rows; i++) {
		for (j = 0; j < activations.cols; j++) {
			blokWin_height = winSideLen;
			blokWin_width = winSideLen;
			blokWin_i = 0;
			blokWin_j = 0;
			win_i = i*stride;
			win_j = j*stride;
			in_i = i*stride - padding;
			in_j = j*stride - padding;
			if (in_i < 0) { blokWin_height += in_i; blokWin_i -= in_i; in_i = 0; }
			if (in_j < 0) { blokWin_width += in_j;  blokWin_j -= in_j; in_j = 0; }
			if (win_i + winSideLen > in_height + padding) {
				blokWin_height -= (win_i + winSideLen - padding - in_height);
			}
			if (win_j + winSideLen > in_width + padding) {
				blokWin_width -= (win_j + winSideLen - padding - in_width);
			}
			window = Matrix(winSideLen, winSideLen);
			window.block(blokWin_i, blokWin_j, blokWin_height, blokWin_width).assign(prevLyr->activations.block(in_i, in_j, blokWin_height, blokWin_width));
			window.reshape(winSideLen* winSideLen, 1);
			weightFaults += window.transpose() * (activations(i, j));
		}
	}
	prevLyr->activations = Matrix(prevLyr->lyrRows, prevLyr->lyrCols);
	for (i = 0; i < activations.rows; i++) {
		for (j = 0; j < activations.cols; j++) {
			blokWin_height = winSideLen;
			blokWin_width = winSideLen;
			blokWin_i = 0;
			blokWin_j = 0;
			win_i = i*stride;
			win_j = j*stride;
			in_i = i*(int )stride - (int)padding;
			in_j = j*(int)stride - (int)padding;
			if (in_i < 0) { blokWin_height += in_i; blokWin_i -= in_i; in_i = 0; }
			if (in_j < 0) { blokWin_width += in_j;  blokWin_j -= in_j; in_j = 0; }
			if (win_i + winSideLen > in_height + padding) {
				blokWin_height -= (win_i + winSideLen - padding - in_height);
			}
			if (win_j + winSideLen > in_width + padding) {
				blokWin_width -= (win_j + winSideLen - padding - in_width);
			}
			tmp = Matrix(blokWin_height, blokWin_width);
			tmp = (weights.reshape(winSideLen, winSideLen)).block(blokWin_i, blokWin_j, blokWin_height, blokWin_width);
			(prevLyr->activations.block(in_i, in_j, blokWin_height, blokWin_width)).reshape(blokWin_height * blokWin_width, 1) += tmp.reshape(blokWin_height * blokWin_width, 1) * (activations(i, j));
		}
	}
	weights.reshape(1, winSideLen * winSideLen) -= weightFaults;
}

void Convolutional::setWeights(const Matrix & mat)
{
	this->weights = mat;
}

void Convolutional::feedFwd()
{
	int in_i, in_j, win_i, win_j, blokWin_i, blokWin_j, blokWin_height, blokWin_width, in_height, in_width;
	Matrix window(winSideLen , winSideLen);
	Matrix tmp;

	in_height = prevLyr->lyrRows;
	in_width = prevLyr->lyrCols;
	for (int i = 0; i < activations.rows; i++) {
		for (int j = 0; j < activations.cols; j++) {
			blokWin_height = winSideLen;
			blokWin_width = winSideLen;
			blokWin_i = 0;
			blokWin_j = 0;
			win_i = i*stride;
			win_j = j*stride;
			in_i = i*stride - padding;
			in_j = j*stride - padding;
			if (in_i < 0) { blokWin_height += in_i; blokWin_i -= in_i; in_i = 0; }
			if (in_j < 0) { blokWin_width += in_j;  blokWin_j -= in_j; in_j = 0; }
			if (win_i + winSideLen > in_height + padding) {
				blokWin_height -= (win_i + winSideLen - padding - in_height);
			}
			if (win_j + winSideLen > in_width + padding) {
				blokWin_width -= (win_j + winSideLen - padding - in_width);
			}
			window = Matrix(winSideLen, winSideLen);
			window.block(blokWin_i, blokWin_j, blokWin_height, blokWin_width).assign(prevLyr->activations.block( in_i, in_j, blokWin_height, blokWin_width));
			cout << window << endl;
			window.reshape(winSideLen* winSideLen, 1);
			cout << window << endl;
			this->activations(i, j) += (weights * window).data[0];
		}
	}
}

//void Convolutional::backProp()
//{
//	Matrix weightFaults(weights.rows, weights.cols);
//
//	for (int i = 0; i < activations.rows; i++) {
//		for (int j = 0; j < activations.cols; j++) {
//
//			this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
//			weightFaults += (inputs.reshape(winSideLen * winSideLen, 1)).transpose() * (activations(i, j));
//		}
//	}
//	for (int i = 0; i < activations.rows; i++) {
//		for (int j = 0; j < activations.cols; j++) {
//			this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
//			this->inputs += weights.transpose() * (activations(i, j));
//		}
//	}
//	weights -= weightFaults;
//}
//
//void Convolutional::feedFwd()
//{
//	for (int i = 0; i < activations.rows; i++) {
//		for (int j = 0; j < activations.cols; j++) {
//			this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
//			weightFaults += (inputs.reshape(winSideLen * winSideLen, 1)).transpose() * (activations(i, j));
//			this->activations(i, j) += (weights * inputs).data[0];
//		}
//	}
//}

//void Convolutional::backProp()
//{
//	Matrix weightFaults(weights.rows, weights.cols);
//	Matrix inputz(winSideLen * winSideLen, 1);
//	unsigned int a, b, c, d, f, g;
//	Matrix e;
//
//	for (int i = 0; i < activations.rows ; i++) {
//		for (int j = 0; j < activations.cols; j++) {
//			inputz = Matrix(winSideLen , winSideLen);
//			c = winSideLen;
//			d = winSideLen;
//			if (i*stride < padding) {
//				a = padding - i*stride;
//				c -= a;
//				f = 0;
//			}
//			else {
//				a = 0;
//				f = (i - padding)*stride;
//			}
//			if (j*stride < padding) {
//				b = padding - j*stride;
//				d -= b;
//				g = 0;
//			}
//			else {
//				b = 0;
//				g = (j - padding)*stride;
//			}
//			if (i*stride + winSideLen > prevLyr->lyrRows + padding) {
//				c -= (i*stride + winSideLen - prevLyr->lyrRows + padding);
//			}
//			if (j*stride + winSideLen > prevLyr->lyrRows + padding) {
//				d -= (j*stride + winSideLen - prevLyr->lyrRows + padding);
//			}
//			e = Matrix(c, d);
//			e = prevLyr->activations.block(f,g, c, d);
//			inputz.block(a, b, c, d) = e;
//			cout << inputz << endl;
//			//this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
//			weightFaults += (inputz.reshape(winSideLen * winSideLen, 1)).transpose() * (activations(i, j)) ;
//			//this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
//			//this->inputs += weights.transpose()* activations.reshape(winSideLen*winSideLen, 1);
//		}
//	}
//	for (int i = 0; i < activations.rows; i++) {
//		for (int j = 0; j < activations.cols; j++) {
//			this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
//			this->inputs += weights.transpose() * (activations(i, j));
//		}
//	}
//	weights -= weightFaults;
//}
//
//void Convolutional::feedFwd()
//{
//	Matrix inputz(winSideLen * winSideLen, 1);
//	unsigned int a, b, c, d, f, g;
//	Matrix e;
//
//	for (int i = 0; i < activations.rows; i++) {
//		for (int j = 0; j < activations.cols; j++) {
//			inputz = Matrix(winSideLen, winSideLen);
//			c = winSideLen;
//			d = winSideLen;
//			if (i*stride < padding) {
//				a = padding - i*stride;
//				c -= a;
//				f = 0;
//			}
//			else {
//				a = 0;
//				f = (i - padding)*stride;
//			}
//			if (j*stride < padding) {
//				b = padding - j*stride;
//				d -= b;
//				g = 0;
//			}
//			else {
//				b = 0;
//				g = (j - padding)*stride;
//			}
//			if (i*stride + winSideLen > prevLyr->lyrRows + padding) {
//				c -= (i*stride + winSideLen - prevLyr->lyrRows + padding);
//			}
//			if (j*stride + winSideLen > prevLyr->lyrRows + padding) {
//				d -= (j*stride + winSideLen - prevLyr->lyrRows + padding);
//			}
//			e = Matrix(c, d);
//			e = prevLyr->activations.block(f, g, c, d);
//			inputz.block(a, b, c, d) = e;
//			cout << inputz << endl;
//			//this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
//			weightFaults += (inputz.reshape(winSideLen * winSideLen, 1)).transpose() * (activations(i, j));
//			this->activations(i, j) += (weights * inputz).data[0];
//		}
//	}
//
//
//	//for (int i = 0; i < activations.rows; i++) {
//	//	for (int j = 0; j < activations.cols; j++) {
//	//		//a = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
//	//		this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1); 
//	//		this->activations(i, j) += (weights * inputs).data[0];
//	//	}
//	//}
//}
