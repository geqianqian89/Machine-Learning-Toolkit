#ifndef ADABOOSTDUAL_H_INCLUDED
#define ADABOOSTDUAL_H_INCLUDED
#include "perceptron.h"

int adaboostdual(Sample *sample, double ***alpha, vector<double> margin, vector<double> bias, vector<double> alfa, Point ****points_temp, int verbose);

#endif // ADABOOSTDUAL_H_INCLUDED
