#pragma once
//#include "../Eigen/Core"
#include "Layer.h"
#include <vector>

//using namespace Eigen;
using namespace std;

class Layer;

class Network
{
public:
	Network(unsigned int numNeuronsFirstLyr, VectorXd numNrnsInHiddenLyrs);
	~Network();
	bool inputConnected;
	bool outputConnected;
	vector<Layer> layers;

	void connect(Network *postNet);
	void feedFwd();
	void backProp(const VectorXd &outputAnswers);
	void correct();
private:
	unsigned int numLyrs;


};

