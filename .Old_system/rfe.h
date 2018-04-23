/*****************************************************
 * recursive feature elimination lib                 *
 *                                                   *
 * Saul Leite <lsaul@lncc.br>                        *
 * sep 23, 2004                                      *
 *****************************************************/
#ifndef RFE_H_INCLUDED
#define RFE_H_INCLUDED
#include "utils.h" 

struct rfe_select_weight{
	double w;
	double val;
	int fname;
};

Sample* rfe_select_features(string filename, Sample *sample, int (*train)(Sample*, double**, double&, int&, int), int depth, int jump, int leave_one_out, int skip, crossvalidation *cv, int verbose);
bool rfe_select_compare_weight_greater(rfe_select_weight a, rfe_select_weight b);

#endif //RFE_H_INCLUDED
