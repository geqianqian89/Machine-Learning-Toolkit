#ifndef PL_H
#define PL_H
#include "data.h"

int linear_programming(Sample *sample, double **w, double &margin, int &svs, int verbose);

#endif //PL_H
