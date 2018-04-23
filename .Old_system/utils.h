/*****************************************************
 * Utils lib                                         *
 *                                                   *
 * Saul Leite <lsaul@lncc.br>                        *
 * Saulo Moraes <saulomv@gmail.com>                  *
 * 2004 / 2011                                       *
 *****************************************************/
#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include "data.h"

struct crossvalidation{
	int fold;
	int qtd;
	int jump;
	int *seed;
	double initial_error;
	double actual_error;
	double limit_error;
};

double* utils_get_weight(Sample *sample);
double* utils_get_dualweight(Sample *sample);
double* utils_get_dualweight_prodint(Sample *sample);
double utils_norm(vector<double> v, double q);

//Algorithm testing functions
double utils_leave_one_out(Sample *sample, int (*train)(Sample*, double**, double&, int&, int), int skip, int verbose);
double utils_k_fold(Sample *sample, int (*train)(Sample*, double**, double&, int&, int), int fold, int seed, int verbose);
void utils_validation(Sample *train_sample, Sample *test_sample, int (*train)(Sample*, double**, double&, int&, int), int fold, int qtde, int verbose);

#endif // UTILS_H_INCLUDED
