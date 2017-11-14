#include "stdafx.h"
#include "Pooling.h"




Pooling::Pooling(PoolingType type)
{
	poolingType = type;
}

Pooling::~Pooling()
{
}

void Pooling::connect(Layer * prevLyr, unsigned int input2WinRatio, unsigned int prevLyr2ThisLyrRatio)
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

	this->lyrRows = floor((prevLyr->lyrRows + padding * 2 - winSideLen) / stride) + 1;
	this->lyrCols = floor((prevLyr->lyrCols + padding * 2 - winSideLen) / stride) + 1;
	this->activations = Matrix(lyrRows, lyrCols);

	zerosPadding = Matrix(prevLyr->lyrRows + 2 * padding, prevLyr->lyrCols + 2 * padding);
	inputs = Matrix_pr(prevLyr->lyrRows + 2 * padding, prevLyr->lyrCols + 2 * padding);
	inputs.pointTo(zerosPadding);
	//(inputs.block(padding , padding, prevLyr->lyrRows, prevLyr->lyrCols)).pointTo(prevLyr->activations);
	for (int i = padding; i < prevLyr->lyrRows + padding; i++) {
		for (int j = padding; j < prevLyr->lyrCols + padding; j++) {
			inputs.data[i*inputs.cols + j] = &prevLyr->activations.data[(i - padding)*prevLyr->lyrCols + j - padding];
		}
	}
}

#if FLIP
void Pooling::feedFwd()
{
	int in_i, in_j, win_i, win_j, blokWin_i, blokWin_j, blokWin_height, blokWin_width, in_height, in_width;
	//Matrix window(winSideLen, winSideLen);
	//Matrix tmp;
	in_height = prevLyr->lyrRows;
	in_width = prevLyr->lyrCols;
	this->activations *= 0;//  Matrix(this->lyrRows, this->lyrCols);
	for (int i = 0; i < activations.rows; i++) {
		for (int j = 0; j < activations.cols; j++) {
			blokWin_height = (int)winSideLen;
			blokWin_width = (int)winSideLen;
			blokWin_i = 0;
			blokWin_j = 0;
			win_i = i*stride;
			win_j = j*stride;
			in_i = i*stride - padding;
			in_j = j*stride - padding;
			if (in_i < 0) { blokWin_height += in_i; blokWin_i -= in_i; in_i = 0; }
			if (in_j < 0) { blokWin_width += in_j;  blokWin_j -= in_j; in_j = 0; }
			if (win_i + winSideLen > in_height + padding) {
				blokWin_height -= (win_i + (int)winSideLen - (int)padding - in_height);
			}
			if (win_j + winSideLen > in_width + padding) {
				blokWin_width -= (win_j + (int)winSideLen - (int)padding - in_width);
			}
			//window = Matrix(winSideLen, winSideLen);
			//window.block(blokWin_i, blokWin_j, blokWin_height, blokWin_width).assign(prevLyr->activations.block(in_i, in_j, blokWin_height, blokWin_width));
			//this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
			switch (poolingType) {
			case Max:
				this->activations(i, j) += (prevLyr->activations.block(in_i, in_j, blokWin_height, blokWin_width)).max();
				break;
			case Avg:
				this->activations(i, j) += (prevLyr->activations.block(in_i, in_j, blokWin_height, blokWin_width)).avg();
				break;
			}
		}
	}
}

void Pooling::backProp()
{
	Matrix prevLyrActs;
	unsigned int r, c;
	int i, j;
	int in_i, in_j, win_i, win_j, blokWin_i, blokWin_j, blokWin_height, blokWin_width, in_height, in_width;
	in_height = prevLyr->lyrRows;
	in_width = prevLyr->lyrCols;
	prevLyrActs = prevLyr->activations;
	prevLyr->activations = Matrix(prevLyr->lyrRows, prevLyr->lyrCols);
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
			//this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
			switch (poolingType) {
			case Max:
				(prevLyrActs.block(in_i, in_j, blokWin_height, blokWin_width)).maxLocation(r, c);
				(prevLyr->activations.block(in_i, in_j, blokWin_height, blokWin_width))(r, c) += this->activations(i, j);
				/*this->inputs.maxLocation(r, c);
				this->inputs(r, c) += this->activations(i, j);*/
				break;
			case Avg:
				(prevLyr->activations.block(in_i, in_j, blokWin_height, blokWin_width)) += this->activations(i, j) / lyrRows / lyrCols * Matrix::ones(blokWin_height * blokWin_width, 1);
				//this->inputs += this->activations(i, j) / lyrRows / lyrCols * Matrix::ones(winSideLen*winSideLen, 1);
				break;
			}
		}
	}
}

#else 
void Pooling::feedFwd()
{
	unsigned int i, j;
	for (i = 0; i < activations.rows; i++) {
		for (j = 0; j < activations.cols; j++) {
			switch (poolingType) {
			case Max:
				cout << inputs.block(i*stride, j*stride, winSideLen, winSideLen) << endl;
				this->activations(i, j) += (inputs.block(i*stride, j*stride, winSideLen, winSideLen)).max();
				break;
			case Avg:
				this->activations(i, j) += (inputs.block(i*stride, j*stride, winSideLen, winSideLen)).avg();
				break;
			}
		}
	}
}


void Pooling::backProp()
{
	unsigned int i, j, r, c;
	for (i = 0; i < activations.rows; i++) {
		for (j = 0; j < activations.cols; j++) {
			switch (poolingType) {
			case Max:
				(inputs.block(i*stride, j*stride, winSideLen, winSideLen)).maxLocation(r, c);
				(inputs.block(i*stride, j*stride, winSideLen, winSideLen))(r, c) += this->activations(i, j);
				/*this->inputs.maxLocation(r, c);
				this->inputs(r, c) += this->activations(i, j);*/
				break;
			case Avg:
				(inputs.block(i*stride, j*stride, winSideLen, winSideLen)) += this->activations(i, j) / lyrRows / lyrCols * Matrix::ones(winSideLen *  winSideLen, 1);
				//this->inputs += this->activations(i, j) / lyrRows / lyrCols * Matrix::ones(winSideLen*winSideLen, 1);
				break;
			}
		}
	}
	this->zerosPadding *= 0;
}
#endif
