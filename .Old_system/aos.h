/*****************************************************
 * Admissible ordered search selection lib           *
 *                                                   *
 * Saul Leite <lsaul@lncc.br>                        *
 * sep 23, 2004                                      *
 *****************************************************/
#ifndef AOS_H_INCLUDED
#define AOS_H_INCLUDED

#include "aos_hash.h"
#include "aos_heap.h"

struct aos_select_weight{
	int fname;
	int index;
	double w;
	double val;
	double pmargin;
	double radius;
	double dcents;
	double golub;
	double fisher;
};

Sample* aos_select_features(string filename, Sample *sample,
        int (*train)(struct sample*,double**,double*,int*,int),
        int breadth, int depth, double bonus, int cut,
        int look_ahead_depth, int skip, int startover,
        int doleave_oo, int sorting_shape, int choice_shape,
        crossvalidation *cv, int verbose);

double aos_select_look_ahead(Sample *sample, AOS_Heap *heap, AOS_Heap *hash, vector<int> &fnames_orig, double *w_orig,
        int (*train)(Sample*,double**,double*,int*,int),
        int depth, int level_orig, int look_ahead_depth,
        double bonus, int choice_shape, int verbose);

void aos_select_feature_main_loop(Sample *sample, AOS_Heap **heap,
        AOS_Hash **hash,
        int (*train)(Sample*,double**,double*,int*,int),
        int breadth, double bonus, int* lool, int startdim,
        int look_ahead_depth, double *g_margin, int cut, int skip,
        int startover, int doleave_oo, int depth, int sorting_type,
        int choice_shape, int ftime, crossvalidation *cv, int verbose);
	
bool aos_select_node_equal(aos_select_gamma one, aos_select_gamma two);
bool aos_select_compare_int_greater(int a, int b);
bool aos_select_compare_weight_greater(aos_select_weight a, aos_select_weight b);
bool aos_select_compare_weightradius_greater(aos_select_weight a, aos_select_weight b);
bool aos_select_compare_weightcenter_greater(aos_select_weight a, aos_select_weight b);
bool aos_select_compare_weightradiuscenter_greater(aos_select_weight a, aos_select_weight b);
bool aos_select_compare_weightfisher_greater(aos_select_weight a, aos_select_weight b);
bool aos_select_compare_weightgolub_greater(aos_select_weight a, aos_select_weight b);

#endif // AOS_H_INCLUDED
