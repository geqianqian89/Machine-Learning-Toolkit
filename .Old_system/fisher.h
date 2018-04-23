/*****************************************************
 * fisher feature selection                          *
 *                                                   *
 * Saulo Moraes Villela <saulomv@gmail.com>          *
 * sep 08, 2011                                      *
 *****************************************************/
#ifndef FISHER_H_INCLUDED
#define FISHER_H_INCLUDED

struct fisher_select_score{
	int fname;
	double score;
};

Sample* fisher_select_features(string filename, Sample *sample, int(*train)(Sample*, double**, double&, int&, int), int number, int verbose);
bool fisher_select_compare_score_greater(fisher_select_score a, fisher_select_score b);

#endif // FISHER_H_INCLUDED
