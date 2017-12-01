// NeuralNetworkEngine.cpp : Defines the entry point for the console application.
//
#define WIN32_LEAN_AND_MEAN



#include "stdafx.h"
#include <boost/asio.hpp>
#include <iostream>

#include <math.h>



#include "MLP.h"
#include "Convolutional.h"
#include "Pooling.h"
#include "ElemWise.h"
#include "cc.h"



using namespace std;


//#pragma pack(push, 1)
//typedef struct FitnessSendMsg {
//	char b;
//	uint32_t batchNum;
//	double fitness;
//	double learningRate;
//	FitnessSendMsg() {
//		b = 'b';
//		batchNum = 0;
//		learningRate = LEARNING_RATE;
//	}
//};
//#pragma pack(pop)

//void printMat(boost::asio::ip::tcp::socket & socket, const Matrix & mat, uint32_t id, uint32_t batchNum);
//extern NNE_Helper NNE_Helper;

int main()
{
	//uint32_t batchNum = 1;
	double fitness = 0;
	//network stuff
	MLP input(IMG_DIM, IMG_DIM, LEARNING_RATE);

	int i, u, ii = 0, iii = 0, iiii = 0;
	int rightDigit, guessDigit;

	Convolutional * conv[NUM_CONVS];
	Pooling * pool[NUM_CONVS];
	ElemWise * bias[NUM_CONVS];
	ElemWise * sig[NUM_CONVS];

	Convolutional * conv2[NUM_CONVS*NUM_BRANCHES];
	Pooling * pool2[NUM_CONVS*NUM_BRANCHES];
	ElemWise * bias2[NUM_CONVS*NUM_BRANCHES];
	ElemWise * sig2[NUM_CONVS*NUM_BRANCHES];

	Convolutional *conv3[NUM_CONVS*NUM_BRANCHES*NUM_BRANCHES];
	Pooling *pool3[NUM_CONVS*NUM_BRANCHES*NUM_BRANCHES];
	ElemWise *bias3[NUM_CONVS*NUM_BRANCHES*NUM_BRANCHES];
	ElemWise *sig3[NUM_CONVS*NUM_BRANCHES*NUM_BRANCHES];

	//classification
	MLP classification1(5, 5, LEARNING_RATE);
	ElemWise bias_class1(Bias, LEARNING_RATE);
	ElemWise sig_class1(Sig, LEARNING_RATE);
	MLP classification2(NUM_OUTS, 1, LEARNING_RATE);
	ElemWise bias_class2(Bias, LEARNING_RATE);
	ElemWise sig_class2(Sig, LEARNING_RATE);

	Layer *layers[NUM_LAYERS];


	//server stuff
	Matrix buffy(NUM_OUTS, 1);
	//FitnessSendMsg fitSndMsg[1];
	size_t msgSize = 0;
	char msgHeader[1];
	char x[1] = { 'x' };
	bool exitFlag = false;
	bool resetFlag = false;
	double lr[1];
	char p[1] = { 'p' };

	//Matrix_pr::result_ = new Matrix_pr;//
	Matrix_pr::result_->data.reserve(IMG_SIZE);
	//Matrix::result_ = new Matrix;//
	Matrix::result_->data.reserve(IMG_SIZE);

	for (i = 0; i < NUM_CONVS; i++) {

		conv[i] = new Convolutional(LEARNING_RATE);
		bias[i] = new ElemWise(Bias, LEARNING_RATE);
		sig[i] = new ElemWise(Sig, LEARNING_RATE);
		pool[i] = new Pooling(Max);


		layers[ii++] = conv[i];
		layers[ii++] = bias[i];
		layers[ii++] = sig[i];
		layers[ii++] = pool[i];


		conv[i]->connect(&input, 4, 1);
		bias[i]->connect(conv[i]);
		sig[i]->connect(bias[i]);
		pool[i]->connect(sig[i], 2, 2);

		for (int j = 0; j < NUM_BRANCHES; j++) {
			conv2[i*NUM_BRANCHES + j] = new Convolutional(LEARNING_RATE);
			bias2[i*NUM_BRANCHES + j] = new ElemWise(Bias, LEARNING_RATE);
			sig2[i*NUM_BRANCHES + j] = new ElemWise(Sig, LEARNING_RATE);
			pool2[i*NUM_BRANCHES + j] = new Pooling(Max);


			layers[ii++] = conv2[i*NUM_BRANCHES + j];
			layers[ii++] = bias2[i*NUM_BRANCHES + j];
			layers[ii++] = sig2[i*NUM_BRANCHES + j];
			layers[ii++] = pool2[i*NUM_BRANCHES + j];

			conv2[i*NUM_BRANCHES + j]->connect(pool[i], 3, 1);
			bias2[i*NUM_BRANCHES + j]->connect(conv2[i*NUM_BRANCHES + j]);
			sig2[i*NUM_BRANCHES + j]->connect(bias2[i*NUM_BRANCHES + j]);
			pool2[i*NUM_BRANCHES + j]->connect(sig2[i*NUM_BRANCHES + j], 2, 2);
			//classification1.connect(pool2[i*NUM_BRANCHES + j]);

			for (int k = 0; k < NUM_BRANCHES; k++) {
				conv3[i*NUM_BRANCHES*NUM_BRANCHES + j*NUM_BRANCHES + k] = new Convolutional(LEARNING_RATE);
				bias3[i*NUM_BRANCHES*NUM_BRANCHES + j*NUM_BRANCHES + k] = new ElemWise(Bias, LEARNING_RATE);
				sig3[i*NUM_BRANCHES*NUM_BRANCHES + j*NUM_BRANCHES + k] = new ElemWise(Sig, LEARNING_RATE);
				pool3[i*NUM_BRANCHES*NUM_BRANCHES + j*NUM_BRANCHES + k] = new Pooling(Max);


				layers[ii++] = conv3[i*NUM_BRANCHES*NUM_BRANCHES + j*NUM_BRANCHES + k];
				layers[ii++] = bias3[i*NUM_BRANCHES*NUM_BRANCHES + j*NUM_BRANCHES + k];
				layers[ii++] = sig3[i*NUM_BRANCHES*NUM_BRANCHES + j*NUM_BRANCHES + k];
				layers[ii++] = pool3[i*NUM_BRANCHES*NUM_BRANCHES + j*NUM_BRANCHES + k];

				conv3[i*NUM_BRANCHES*NUM_BRANCHES + j*NUM_BRANCHES + k]->connect(pool2[i*NUM_BRANCHES + j], 3, 1);
				bias3[i*NUM_BRANCHES*NUM_BRANCHES + j*NUM_BRANCHES + k]->connect(conv3[i*NUM_BRANCHES*NUM_BRANCHES + j*NUM_BRANCHES + k]);
				sig3[i*NUM_BRANCHES*NUM_BRANCHES + j*NUM_BRANCHES + k]->connect(bias3[i*NUM_BRANCHES*NUM_BRANCHES + j*NUM_BRANCHES + k]);
				pool3[i*NUM_BRANCHES*NUM_BRANCHES + j*NUM_BRANCHES + k]->connect(sig3[i*NUM_BRANCHES*NUM_BRANCHES + j*NUM_BRANCHES + k], 2, 2);
				classification1.connect(pool3[i*NUM_BRANCHES*NUM_BRANCHES + j*NUM_BRANCHES + k]);
			}
		}
	}
	bias_class1.connect(&classification1);
	sig_class1.connect(&bias_class1);

	classification2.connect(&sig_class1);
	bias_class2.connect(&classification2);
	sig_class2.connect(&bias_class2);

	layers[ii++] = &classification1;
	layers[ii++] = &bias_class1;
	layers[ii++] = &sig_class1;
	layers[ii++] = &classification2;
	layers[ii++] = &bias_class2;
	layers[ii++] = &sig_class2;

	//name the layers
	input.name = "L";
	for (int i = 0; i < ii; i++)
	{
		layers[i]->name = layers[i]->prevLyr->name;
		layers[i]->name.append("_");
		layers[i]->name.append(to_string(layers[i]->prevLyr->tmpChildIter));
		layers[i]->prevLyr->tmpChildIter++;
	}
	for (int i = 0; i < ii; i++)
	{
		layers[i]->name.append("_");
		switch (layers[i]->layerType) {
		case mlp:
			layers[i]->name.append("mlp");
			break;
		case convolutional:
			layers[i]->name.append("conv");
			break;
		case pooling:
			layers[i]->name.append("pool");
			break;
		}
	}


	while (!exitFlag) {
		//block until client connects
		std::cout << "Neural Network Engine (NNE) Server is running\n";
		NNE_helper.acceptor_.accept(NNE_helper.socket_);
		std::cout << "Client has accepted connection. Awaiting message\n";
		exitFlag = false;
		//while the user hasn't sent the exit code or disconnected the client
		while (!exitFlag && !resetFlag) {
			//listen for messages from the client 
			if ((msgSize = NNE_helper.socket_.available()) > 0) {
				//read the message header: a single char to determine message type
				read(NNE_helper.socket_, boost::asio::buffer(msgHeader, 1));
				switch (*msgHeader) {
				case 'p':
					for (ii = 0; ii < NUM_LAYERS; ii++) {
						if (layers[ii]->layerType == pooling) {
							NNE_helper.printMat(layers[ii]->activations, layers[ii]->name + "_act");
						}
					}
					NNE_helper.printMat(layers[NUM_LAYERS - 1]->activations, "results");
					write(NNE_helper.socket_, boost::asio::buffer(p, sizeof(p)));
					break;
				case 'f':
					for (ii = 0; ii < NUM_LAYERS; ii++) {
						if (layers[ii]->layerType == pooling) {
							NNE_helper.printMat(layers[ii]->activations, layers[ii]->name + "_act");
						}
					}
					write(NNE_helper.socket_, boost::asio::buffer(p, sizeof(p)));
					break;
				case 'l':
					read(NNE_helper.socket_, boost::asio::buffer(lr, sizeof(double)));
					conv[3]->setLearningRate(lr[0]);
					conv[2]->setLearningRate(lr[0]);
					conv[1]->setLearningRate(lr[0]);
					conv[0]->setLearningRate(lr[0]);
					classification1.setLearningRate(lr[0]);
					classification2.setLearningRate(lr[0]);
					cout << "changing learning rate" << endl;
					break;
					//training data
				case 't':
					NNE_helper.newBatch();
					//read in the inputBatch data
					read(NNE_helper.socket_, boost::asio::buffer(input.activations.data, IMG_SIZE * sizeof(double)));
					input.activations *= (1.0 / 255.0);
					//cout << input.activations << endl;
					
					for (ii = 0; ii < NUM_LAYERS; ii++) {
						layers[ii]->feedFwd();
					}
					

					read(NNE_helper.socket_, boost::asio::buffer(buffy.data, NUM_OUTS * sizeof(double)));

					iii = 0;
					for (ii = 0; ii < NUM_OUTS; ii++) {
						if (buffy.data[ii] == 1) {
							cout << iii << ": ";
							rightDigit = iii;
						}
						iii++;
					}
					iiii = 0;
					for ( ii = 0; ii < NUM_OUTS; ii++) {
						if (layers[NUM_LAYERS - 1]->activations.data[ii] > layers[NUM_LAYERS - 1]->activations.data[iiii]) {
							iiii = ii;
						}
					}
					cout << iiii << ": ";
					layers[NUM_LAYERS - 1]->dEdA = layers[NUM_LAYERS - 1]->activations - buffy;

					fitness = 0;
					for ( u = 0; u < layers[NUM_LAYERS - 1]->activations.data.size(); u++) {
						fitness += pow(layers[NUM_LAYERS - 1]->dEdA(u), 2.0);
					}

					NNE_helper.printMat(fitness, "fitness");
					cout << std::setw(8) << std::setprecision(2) << fitness;
					if (iiii != rightDigit) {
						cout << " x ";
					}		
					cout << endl;

					//backprop
					for ( ii = NUM_LAYERS - 1; ii >= 0; ii--) {
						layers[ii]->backProp();						
					}
					
					break;
				case 'x':
					exitFlag = true;
					write(NNE_helper.socket_, boost::asio::buffer(x, 1));
					NNE_helper.socket_.close();
					for (int i = 0; i < NUM_LAYERS; i++) {
						//delete &layers[i];
					}
					cout << "exiting" << endl;
					break;
				default:
					std::cout << "Unrecognized message header: " << msgHeader << endl;
					break;
				}
			}
		}
	}
	//conv1.feedFwd();
	//pool1.feedFwd();

	//pool1.activations = Matrix::ones(pool1.lyrRows, pool1.lyrCols);
	//pool1.backProp();
	//conv1.backProp();

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

//void printMat(boost::asio::ip::tcp::socket & socket, const Matrix & mat, uint32_t id, uint32_t batchNum) {
//	static MatrixSendMsg matSndMsg[1];
//
//	matSndMsg->id = id;
//	matSndMsg->batchNum = batchNum;
//	matSndMsg->rows = mat.rows;
//	matSndMsg->cols = mat.cols;
//
//	write(socket, boost::asio::buffer(matSndMsg, sizeof(MatrixSendMsg)));
//	write(socket, boost::asio::buffer(mat.data.data(), mat.data.size() * sizeof(double)));
//}