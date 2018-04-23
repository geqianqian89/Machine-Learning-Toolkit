#ifndef ALMAP_H_INCLUDED
#define ALMAP_H_INCLUDED
#include "perceptron.h"

int almap(Sample *sample, double **w, double& margin, int& svs, int verbose);

#endif //ALMAP_H_INCLUDED
