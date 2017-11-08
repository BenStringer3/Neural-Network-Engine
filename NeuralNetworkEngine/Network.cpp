#include "stdafx.h"
#include "Network.h"




Network::Network(unsigned int numNeuronsFirstLyr, VectorXd numNrnsInHiddenLyrs)
{
	int i;
	inputConnected = false;
	outputConnected = false;


	layers.emplace_back(Layer(numNeuronsFirstLyr));
	for (i = 0; i < numNrnsInHiddenLyrs.rows(); i++) {
		if (numNrnsInHiddenLyrs(i) != 0) {
			layers.emplace_back(Layer(numNrnsInHiddenLyrs(i)));
		}
	}

	numLyrs = layers.size();

	for (i = 0; i < numLyrs - 1; i++) {
		layers[i].connect(&layers[i + 1]);
	}
}

Network::~Network()
{
}

void Network::connect(Network * postNet)
{
	this->layers.back().connect(&postNet->layers[0]);

	this->outputConnected = true;
	postNet->inputConnected = true;
}


void Network::feedFwd()
{
	
	for (int i = 0; i < numLyrs; i++) {
		layers[i].feedFwd();
	}
}

void Network::backProp(const VectorXd &outputAnswers)
{
	int i;
	if (outputConnected) {
		for (i = numLyrs - 1; i >= 0; i--) {
			layers[i].backProp();
		}
	}
	else {
		layers.back().dels = layers.back().acts - outputAnswers;
		std::cout << layers.back().dels << std::endl;
		for (i = numLyrs - 2; i >= 0; i--) {
			layers[i].backProp();
		}
	}
		
	for (i = 0; i < numLyrs; i++) {
		layers[i].accum();
	}
}

void Network::correct() {
	int i;
	for (i = 0; i < numLyrs; i++) {
		layers[i].correct();
	}
}



