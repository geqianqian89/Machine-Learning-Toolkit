/*****************************************************
 * SMO classifier lib                                *
 *****************************************************/
 
#ifndef SMO_H_INCLUDED
#define SMO_H_INCLUDED
#include <list>

struct smo_learning_data{
	double error;
	char done;
	list<int> sv;
};

int smo_examine_example(Sample *sample, smo_learning_data *l_data, double** matrix, list<int> head, int i1, int verbose);
int smo_max_errors(Sample *sample, smo_learning_data *l_data, double **matrix, list<int> head, int i1, double e1, int verbose);
int smo_iterate_non_bound(Sample *sample, smo_learning_data *l_data, double** matrix, list<int> head, int i1, int verbose);
int smo_iterate_all_set(Sample *sample, smo_learning_data *l_data, double** matrix, list<int> head, int i1, int verbose);
int smo_take_step(Sample *sample, smo_learning_data *l_data, double** matrix, list<int> head, int i1, int i2, int verbose);
double smo_function(Sample *sample, double** matrix, list<int> head, int index);
void smo_test_learning(Sample *sample, double **matrix, smo_learning_data *l_data, list<int> head);
int smo_train_matrix(Sample *sample, double **matrix, double &margin, int &svs, int verbose); 
int smo_train(Sample *sample, double **w, double &margin, int &svs, int verbose);
int smo_training_routine(Sample *sample, smo_learning_data* l_data, double** matrix, list<int> head,int verbose);

#endif // SMO_H_INCLUDED

