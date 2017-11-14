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
//#include "Network.h"

#define IMG_DIM 28
#define NUM_OUTS 10
#define IMG_SIZE 784

using namespace std;

#pragma pack(push, 1)
typedef struct MatrixSendMsg {
	char m;
	uint32_t rows;
	uint32_t cols;
	MatrixSendMsg() {
		m = 'm';
	}
};

typedef struct FitnessSendMsg {
	char b;
	uint32_t batchNum;
	double fitness;
	FitnessSendMsg() {
		b = 'b';
		batchNum = 0;
	}
};
#pragma pack(pop)

int main()
{
	//network stuff
	MLP input(IMG_DIM, IMG_DIM);
	Convolutional conv1;
	Pooling pool1(Max);
	Convolutional conv2;
	Pooling pool2(Max);
	MLP classification1(5, 5);
	MLP classification2(NUM_OUTS, 1);

	//server stuff
	Matrix buffy(NUM_OUTS, 1);
	MatrixSendMsg matSndMsg[1];
	FitnessSendMsg fitSndMsg[1];
	uint32_t trainingDataSizeMsg[1];
	size_t msgSize = 0;
	char msgHeader[1];
	char x[1] = { 'x' };
	bool exitFlag = false;
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 4012));
	boost::asio::ip::tcp::socket socket(io_service);

	input.activations = Matrix::random(input.activations.rows, input.activations.cols);

	//connect the network
	conv1.connect(&input, 5, 2);
	pool1.connect(&conv1, 5, 2);
	//conv2.connect(&input, 3, 2);
	//pool2.connect(&conv2, 3, 2);
	classification1.connect(&pool1);
	classification1.connect(&pool2);
	classification2.connect(&classification1);

	while (1) {
		//block until client connects
		std::cout << "Neural Network Engine (NNE) Server is running\n";
		acceptor.accept(socket);
		std::cout << "Client has accepted connection. Awaiting message\n";
		exitFlag = false;
		//while the user hasn't sent the exit code or disconnected the client
		while (!exitFlag) {
			//listen for messages from the client
			if ((msgSize = socket.available()) > 0) {
				//read the message header: a single char to determine message type
				read(socket, boost::asio::buffer(msgHeader, 1));
				switch (*msgHeader) {
					//training data
				case 't':
					//cout << "Receiving training data" << endl;
					//std::cout << "Bytes available: " << (msgSize = socket.available()) << endl;
					//read(socket, boost::asio::buffer(trainingDataSizeMsg, sizeof(trainingDataSizeMsg)));
					//std::cout << "batchSize: " << trainingDataSizeMsg << endl;

					//read in the inputBatch data
					read(socket, boost::asio::buffer(input.activations.data, IMG_SIZE * sizeof(double)));
					//cout << "read socket. now " << (msgSize = socket.available()) << " bytes available" << endl;

					conv1.feedFwd();
					pool1.feedFwd();
					//conv2.feedFwd();
					//pool2.feedFwd();
					classification1.feedFwd();
					classification2.feedFwd();

					//cout << pool1.activations << endl;
					matSndMsg->m = 'm';
					matSndMsg->rows = classification2.lyrRows;
					matSndMsg->cols = classification2.lyrCols;
					write(socket, boost::asio::buffer(matSndMsg, sizeof(matSndMsg)));
					write(socket, boost::asio::buffer(classification2.activations.data, classification2.activations.data.size() * sizeof(double) ));
					
					matSndMsg->rows = conv1.lyrRows;
					matSndMsg->cols = conv1.lyrCols;
					matSndMsg->m = 'r';
					write(socket, boost::asio::buffer(matSndMsg, sizeof(matSndMsg)));
					write(socket, boost::asio::buffer(conv1.activations.data, conv1.activations.data.size() * sizeof(double)));

					read(socket, boost::asio::buffer(buffy.data, NUM_OUTS * sizeof(double)));
					//cout << "read socket. now " << (msgSize = socket.available()) << " bytes available" << endl;
					classification2.activations -= buffy;
					//cout << classification2.activations << endl;
					fitSndMsg->fitness = 0;
					for (int u = 0; u < classification2.activations.data.size(); u++) {
						fitSndMsg->fitness += pow(classification2.activations(u), 2.0);
					}
					fitSndMsg->batchNum++;
					write(socket, boost::asio::buffer(fitSndMsg, sizeof(fitSndMsg)));
					classification2.backProp();
					classification1.backProp();
					pool1.backProp();
					conv1.backProp();
					//pool2.backProp();
					//conv2.backProp();
					break;
				case 'x':
					exitFlag = true;
					write(socket, boost::asio::buffer(x, 1));
					socket.close();
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