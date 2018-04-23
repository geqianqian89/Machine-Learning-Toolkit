/*****************************************************
 * golub feature selection                           *
 *                                                   *
 * Saul Leite <lsaul@lncc.br>                        *
 * sep 23, 2004                                      *
 *****************************************************/
#ifndef GOLUB_H_INCLUDED
#define GOLUB_H_INCLUDED

struct golub_select_score{
	int fname;
	double score;
};

Sample* golub_select_features(string filename, Sample *sample, int (*train)(Sample*, double**, double&, int&, int), int number, int verbose);
bool golub_select_compare_score_greater(golub_select_score a, golub_select_score b);

#endif
