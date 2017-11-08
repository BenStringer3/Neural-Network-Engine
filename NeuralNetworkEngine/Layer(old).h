#pragma once
//#include "../Eigen/Core"
#include "Matrix.h"
#include <iostream>
//#include "Network.h"

//using namespace Eigen;



class Layer
{
public:
	Layer(unsigned int numNrns);
	Layer(unsigned int rows, unsigned int cols);
	~Layer();

	void connect(Layer *postLyr);
	void connect(Layer * postLyr, unsigned int inStartRow, unsigned int inStartCol, unsigned int inHeight, unsigned int inWidth, unsigned int outStartRow, unsigned int outStartCol, unsigned int outHeight, unsigned int outWidth);
	void feedFwd();
	void backProp();
	void accum();
	void correct();
	void shift(int upDown, int leftRight);
	Matrix acts; // j x 1
	Matrix dels; // j x 1
				   //i: number of neurons in previous layer
	unsigned int j; //j: number of neurons in layer
					//k: number of neurons in next layer

	//Matrix *inputs;
	Matrix_pr inputs;

	Matrix weights; // j x i
	Matrix weightFaults; // j x i

	Matrix *postLyrWeights;
	Matrix *postLyrDels;

	Matrix z; // j x 1
	Matrix biases; // j x 1
	Matrix biasFaults; // j x 1
	static Matrix sig(Matrix *x);
	Matrix sig_prime(Matrix *x);
private:
	unsigned int rows; 
	unsigned int cols;

	unsigned int winRows;
	unsigned int winCols;
	unsigned int winStartRow;
	unsigned int winStartCol;
};

