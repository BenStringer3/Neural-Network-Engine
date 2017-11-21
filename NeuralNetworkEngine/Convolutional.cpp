#include "stdafx.h"
#include "Convolutional.h"
#include "cc.h"

#define FLIP 0

Convolutional::Convolutional(double learningRate)
{
	this->learningRate = learningRate;
	iter = 1;
	iterPerEpoch = 30;
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
	/*if (prevLyr->lyrRows < prevLyr->lyrCols) { 
		this->winSideLen = prevLyr->lyrRows / input2WinRatio;
	}
	else {
		this->winSideLen = prevLyr->lyrCols / input2WinRatio;
	}*/
	this->winSideLen = winSideLen;
	this->stride = prevLyr2ThisLyrRatio;

	padding = (unsigned int)floor(winSideLen / 2);

	this->weights = Matrix::random(1, winSideLen*winSideLen);
	this->dEdW = Matrix(weights.rows, weights.cols);
	this->inertial_dEdW = Matrix(weights.rows, weights.cols);
	this->correctionCoeffs = Matrix(weights.rows, weights.cols);

	this->lyrRows = (unsigned int)floor((prevLyr->lyrRows + padding * 2 - winSideLen) / stride) + 1 ;
	this->lyrCols = (unsigned int)floor((prevLyr->lyrCols + padding * 2 - winSideLen) / stride) + 1;
	this->activations = Matrix(lyrRows, lyrCols);
this->biases = Matrix::random(this->lyrRows, this->lyrCols);
dEdB = Matrix(lyrRows, lyrCols);
inertial_dEdB = Matrix(lyrRows, lyrCols);

	this->dEdA = Matrix(lyrRows, lyrCols);
	//this->activations = Matrix((prevLyr->lyrRows  - winSideLen) / stride , (prevLyr->lyrCols  - winSideLen) / stride);
	
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

#if !FLIP
void Convolutional::backProp()
{
	unsigned int i, j;
	//dEdW *= 0;

	for (i = 0; i < activations.rows; i++) {
		for (j = 0; j < activations.cols; j++) {
			//cout << endl;
			//for (int ii = 0; ii < inputs.rows; ii++) {
			//for (int jj = 0; jj < inputs.rows; jj++) {
			//if ((jj == j*stride || jj == j*stride + winSideLen) && (i*stride <= ii) && (ii < (i*stride + winSideLen))) {
			//cout << "|";
			//}

			//cout << std::setw(5) << std::fixed << std::setprecision(2) << inputs(ii, jj) << " ";


			//}
			//cout << endl;
			//}
			//cout << dEdA(i, j) << endl;
			//cout << weights << endl;
			dEdW += (inputs.block(i*stride, j*stride, winSideLen, winSideLen)).reshape(1, winSideLen* winSideLen) * (dEdA(i, j));
			//cout << dEdW << endl;
			(prevLyrDels.block(i*stride, j*stride, winSideLen, winSideLen)).reshape(winSideLen * winSideLen, 1) += weights.transpose() * (dEdA(i, j));
			zerosPadding *= 0;
		}
	}

	//this->correctionCoeffs = 0.9*this->correctionCoeffs + learningRate*dEdW;
	dEdB += dEdA;
	
	if (iter == iterPerEpoch) {
		inertial_dEdB = (INERTIA * inertial_dEdB + learningRate*dEdB / iter);
		biases -= inertial_dEdB;

		inertial_dEdW = (INERTIA * inertial_dEdW + learningRate*dEdW / iter);
		weights -= inertial_dEdW; ;// this->correctionCoeffs;
		
		dEdW *= 0;
		dEdB *= 0;
		iter = 0;
	}
	iter++;
}

void Convolutional::feedFwd() {
	unsigned int i, j;
	for (i = 0; i < activations.rows; i++) {
		for (j = 0; j < activations.cols; j++) {
			//window.block(blokWin_i, blokWin_j, blokWin_height, blokWin_width).assign(prevLyr->activations.block(in_i, in_j, blokWin_height, blokWin_width));

			//window.reshape(winSideLen* winSideLen, 1);
			//cout << inputs.block(i*stride, j*stride, winSideLen, winSideLen) << endl;

			/*cout << endl;
			for (int ii = 0; ii < inputs.rows; ii++) {
				for (int jj = 0; jj < inputs.rows; jj++) {
					if ((jj == j*stride || jj == j*stride + winSideLen) && (i*stride <= ii) && (ii < (i*stride + winSideLen))) {
						cout << "|";
					}

						cout << std::setw(5) << std::fixed << std::setprecision(2) << inputs(ii, jj) << " ";


				}
				cout << endl;
			}*/
			//cout << weights << endl;
			this->activations(i, j) = (weights * (inputs.block(i*stride, j*stride, winSideLen, winSideLen)).reshape(winSideLen* winSideLen, 1)).data[0] +biases(i, j);
			//cout << this->activations << endl;
		}
	}
	dEdA *= 0;
	//NNE_helper.printMat(activations);
	//inputs *= 0;
}
#endif

#if FLIP
void Convolutional::backProp()
{
	Matrix dEdW(weights.rows, weights.cols);
	unsigned int i, j;
	int in_i, in_j, win_i, win_j, blokWin_i, blokWin_j, blokWin_height, blokWin_width, in_height, in_width;
	Matrix window(winSideLen, winSideLen);
	//Matrix tmp;

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
			window *= 0;// Matrix(winSideLen, winSideLen);
			window.block(blokWin_i, blokWin_j, blokWin_height, blokWin_width).assign(prevLyr->activations.block(in_i, in_j, blokWin_height, blokWin_width));
			
			dEdW += window.reshape(1, winSideLen* winSideLen) * (activations(i, j));
		}
	}
	prevLyr->activations *= 0;// Matrix(prevLyr->lyrRows, prevLyr->lyrCols);
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
			//tmp = Matrix(blokWin_height, blokWin_width);
			//tmp = (weights.reshape(winSideLen, winSideLen)).block(blokWin_i, blokWin_j, blokWin_height, blokWin_width);
			(prevLyr->activations.block(in_i, in_j, blokWin_height, blokWin_width)).reshape(blokWin_height * blokWin_width, 1) += ((weights.reshape(winSideLen, winSideLen)).block(blokWin_i, blokWin_j, blokWin_height, blokWin_width)).reshape(blokWin_height * blokWin_width, 1) * (activations(i, j));
		}
	}
	weights.reshape(1, winSideLen * winSideLen) -= dEdW;
}



void Convolutional::feedFwd()
{
	int in_i, in_j, win_i, win_j, blokWin_i, blokWin_j, blokWin_height, blokWin_width, in_height, in_width;
	Matrix window(winSideLen , winSideLen);
	unsigned int i, j;
	in_height = prevLyr->lyrRows;
	in_width = prevLyr->lyrCols;
	for (i = 0; i < activations.rows; i++) {
		for ( j = 0; j < activations.cols; j++) {
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
			window *= 0;//ss Matrix(winSideLen, winSideLen);
			window.block(blokWin_i, blokWin_j, blokWin_height, blokWin_width).assign(prevLyr->activations.block( in_i, in_j, blokWin_height, blokWin_width));

			//window.reshape(winSideLen* winSideLen, 1);

			this->activations(i, j) += (weights * window.reshape(winSideLen* winSideLen, 1)).data[0];
		}
	}
}
#endif

void Convolutional::setWeights(const Matrix & mat)
{
	this->weights = mat;
}

void Convolutional::setLearningRate(double lr)
{
	this->learningRate = lr;
}

//void Convolutional::backProp()
//{
//	Matrix dEdW(weights.rows, weights.cols);
//
//	for (int i = 0; i < activations.rows; i++) {
//		for (int j = 0; j < activations.cols; j++) {
//
//			this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
//			dEdW += (inputs.reshape(winSideLen * winSideLen, 1)).transpose() * (activations(i, j));
//		}
//	}
//	for (int i = 0; i < activations.rows; i++) {
//		for (int j = 0; j < activations.cols; j++) {
//			this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
//			this->inputs += weights.transpose() * (activations(i, j));
//		}
//	}
//	weights -= dEdW;
//}
//
//void Convolutional::feedFwd()
//{
//	for (int i = 0; i < activations.rows; i++) {
//		for (int j = 0; j < activations.cols; j++) {
//			this->inputs = prevLyr->activations.block(i*stride, j*stride, winSideLen, winSideLen).reshape(winSideLen*winSideLen, 1);
//			dEdW += (inputs.reshape(winSideLen * winSideLen, 1)).transpose() * (activations(i, j));
//			this->activations(i, j) += (weights * inputs).data[0];
//		}
//	}
//}

//void Convolutional::backProp()
//{
//	Matrix dEdW(weights.rows, weights.cols);
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
//			dEdW += (inputz.reshape(winSideLen * winSideLen, 1)).transpose() * (activations(i, j)) ;
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
//	weights -= dEdW;
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
//			dEdW += (inputz.reshape(winSideLen * winSideLen, 1)).transpose() * (activations(i, j));
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
