#pragma once

#include "NNE_Helper.h"
#include <iomanip>

#define INITIAL_BIASES this->biases = Matrix::ones(this->lyrRows, this->lyrCols) * -3;


#define IMG_DIM 28
#define NUM_OUTS 10
#define IMG_SIZE 784
#define INERTIA 0.0
#define BETA1 0.9
#define BETA2 0.999
#define E 10e-9
#define ITER_PER_EPOCH 1

#define NUM_CONVS 5
#define NUM_BRANCHES 2
#define NUM_CLASSIFICATIONS 2
#define NUM_LAYERS (NUM_CLASSIFICATIONS*3 + NUM_CONVS*4 + NUM_CONVS*4*NUM_BRANCHES + NUM_CONVS*4*NUM_BRANCHES *NUM_BRANCHES)

#define LEARNING_RATE 0.05

extern NNE_Helper NNE_helper;