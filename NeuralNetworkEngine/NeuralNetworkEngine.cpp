// NeuralNetworkEngine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <boost/asio.hpp>
#include <iostream>
#include <math.h>
//#include "../Eigen/Core"

#include "MLP.h"
#include "Convolutional.h"
#include "Pooling.h"
//#include "Network.h"

/*using namespace Eigen;*/ 
using namespace std;

int main()
{
	MLP input(5,5);
	Convolutional conv1;
	Pooling pool1(Max);

	input.activations = Matrix::random(5, 5);

	conv1.connect(&input, 1, 2);
	//pool1.connect(&conv1, 3, 3);

	conv1.feedFwd();

	conv1.activations = Matrix::ones(conv1.lyrRows, conv1.lyrCols);

	conv1.backProp();

	//double inputBatch[4][2] = { 0, 0, 0, 1, 1, 0,1,1 };
	//double outputBatch[4] = { 0,0,0,1 };

	//MLP lyr1(2, 1);
	//MLP lyr2(2, 1);
	//MLP lyr3(1, 1);

	//lyr2.connect(&lyr1);
	//lyr3.connect(&lyr2);

	//for (int j = 0; j < 100; j++) {
	//	for (int i = 0; i < 4; i++) {
	//		lyr1.activations(0) = inputBatch[i][0];
	//		lyr1.activations(1) = inputBatch[i][1];

	//		lyr2.feedFwd();
	//		lyr3.feedFwd();

	//		cout << (lyr3.activations(0) -=  outputBatch[i]) << endl;
	//		lyr3.backProp();
	//		lyr2.backProp();
	//	}
	//}

	return 0;
}