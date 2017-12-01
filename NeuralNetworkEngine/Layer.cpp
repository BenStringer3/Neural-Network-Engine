#include "stdafx.h"
#include "Layer.h"
#include "cc.h"

Layer::Layer()
{
	//outputConnected = false;
	iter = 1;
	iterPerEpoch = ITER_PER_EPOCH;
}


Layer::~Layer()
{
}

void Layer::feedFwd()
{
	calculateActivations();
	dEdA *= 0;
}

void Layer::applyCorrections(Matrix & thingToBeCorrected)
{
	if (iter % iterPerEpoch == 0) {
		correctionToBeAccumulated /= iterPerEpoch;
		inertia = BETA1*inertia + (1 - BETA1)*correctionToBeAccumulated;
		velocity = BETA2*velocity + (1 - BETA2)*((correctionToBeAccumulated.cwiseProduct(correctionToBeAccumulated)).sum());
		thingToBeCorrected -= LEARNING_RATE / sqrt(velocity + E)*(sqrt(1 - pow(BETA2, iter)) / (1 - pow(BETA1, iter))*inertia);

		correctionToBeAccumulated *= 0;
	}
	iter++;
}
