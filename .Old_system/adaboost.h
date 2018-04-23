#ifndef ADABOOST_H_INCLUDED
#define ADABOOST_H_INCLUDED
#include <vector>
#include "perceptron.h"

void create_set(vector<double> dist, int **matrix, int line);
int adaboost(Sample *sample, double ***w, vector<double> margin, vector<double> bias, vector<double> alfa, int verbose);

#endif
