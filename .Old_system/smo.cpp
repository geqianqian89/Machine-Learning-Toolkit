/*****************************************************
 * SMO classifier lib                                *
 *                                                   *
 * Saul Leite <lsaul@lncc.br>                        *
 * sep 23, 2004                                      *
 *****************************************************/
 
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iterator>
#include <list>
#include "utils.h"
#include "kernel.h"
#include "data.h"
#include "imadual.h"
#include "smo.h"

#define min(a,b) ((a) < (b)?(a):(b));
#define max(a,b) ((a) > (b)?(a):(b));
#define C 9999
#define EPS 0.0000001
#define TOL 0.0001
#define MAX_EPOCH 9999

extern double kernel_param;
extern int kernel_type;

/*----------------------------------------------------------*
 * Fuction to find a second training example to change      *
 *----------------------------------------------------------*/

int smo_examine_example(Sample *sample, smo_learning_data *l_data, double **matrix, list<int> head, int i1, int verbose){
	int i = 0;
	double y1, e1, r1, alpha1;
	
	y1 = e1 = r1 = alpha1 = 0.0;
	
	//Cleaning up done list
	for(i = 0; i < sample->size; ++i) l_data[i].done = 0;
	l_data[i1].done = 1;
	
	//reading stuff from array
	y1     = sample->points[i1]->y;
    alpha1 = sample->points[i1]->alpha;
    if(alpha1 > 0 && alpha1 < C) e1 = l_data[i1].error;
    else                         e1 = smo_function(sample, matrix, head, i1) - y1;
	
	//Calculating r1
	r1 = y1 * e1;
	
	//Try to find next example by 3 different ways
	if((r1 < -TOL && alpha1 < C) || (r1 > TOL && alpha1 > 0)){
			 if(smo_max_errors(sample,l_data,matrix,head,i1,e1,verbose)) return 1;
		else if(smo_iterate_non_bound(sample,l_data,matrix,head,i1,verbose)) return 1;
		else if(smo_iterate_all_set(sample,l_data,matrix,head,i1,verbose)) return 1;
	}else if(verbose > 2) cout << "Return0 -1" << endl;
	
	return 0;
}


int smo_max_errors(Sample *sample, smo_learning_data *l_data, double **matrix, list<int> head, int i1, double e1, int verbose){
	int k = 0, i2 = -1;
	double tmax, e2, temp;
	list<int>::iterator itr = head.begin();
	tmax = e2 = temp = 0.0;
	
	if(verbose > 2) cout << "Max errors iterations\n";
	
	//iterare through the non-bound examples
	for(itr = head.begin(); itr != head.end(); ++itr){
		k = *(itr);
		if(l_data[k].done == 0 && sample->points[k]->alpha < C){
			e2 = l_data[k].error;
			temp = fabs(e1-e2);
			
			if(temp > tmax){ tmax = temp; i2 = k; }
		}
	}
	
	if(i2 >= 0 && smo_take_step(sample, l_data, matrix, head, i1, i2, verbose)) return 1;
	
	return 0;
}

/*----------------------------------------------------------*
 * Find second example, look at the non-bound examples      *
 *----------------------------------------------------------*/
 
int smo_iterate_non_bound(Sample *sample, smo_learning_data *l_data, double **matrix, list<int> head, int i1, int verbose){
	int k = 0;
	list<int>::iterator itr = head.begin();
	
	if(verbose > 2) cout << "	Non-bound iteration\n";
	
	for(itr = head.begin(); itr != head.end(); ++itr){
		k = *(itr);
		if(l_data[k].done == 0 && sample->points[k]->alpha < C){
			if(smo_take_step(sample, l_data, matrix, head, i1, k, verbose)) return 1;
		}
	}
	
	return 0;
}

/*----------------------------------------------------------*
 * Find second example, look at the entire set              *
 *----------------------------------------------------------*/

int smo_iterate_all_set(Sample *sample, smo_learning_data *l_data, double** matrix, list<int> head, int i1, int verbose){
	int k0 = 0, k = 0, i2 = 0, size = sample->size;
	
	if(verbose > 2) cout << "All-set iteration" << endl;
	
	srand(0);
	k0 = rand()%size;
	
	for(k = k0; k < size+k0; ++k){
		i2 = k%size;
		if(l_data[i2].done == 0 && smo_take_step(sample, l_data, matrix, head, i1, i2, verbose))
			return 1;
	}	
	return 0;
}	

/*----------------------------------------------------------*
 * Change two alphas in the training set                    *
 *----------------------------------------------------------*/
 
int smo_take_step(Sample *sample, smo_learning_data *l_data,
 double** matrix, list<int> head,int i1, int i2, int verbose){
	int i, y1, y2, s;
	double alpha1, alpha2, new_alpha1, new_alpha2;
	double e1, e2, min_val, max_val, eta;
	double max_val_f, min_val_f, bnew, b;
	double t1, t2, error_tot;
	list<int>::iterator itr;
	
	i = y1 = y2 = s = 0;
	alpha1 = alpha2 = new_alpha1 = new_alpha2 = max_val_f = min_val_f = 0.0;
	e1 = e2 = min_val = max_val = eta = t1 = t2 = error_tot = bnew = b = 0.0;
	
	//this sample is done
	l_data[i2].done = 1;
	
	//get info from sample struct
	b      = -sample->bias;
    y1     = sample->points[i1]->y;
    y2     = sample->points[i2]->y;
    alpha1 = sample->points[i1]->alpha;
    alpha2 = sample->points[i2]->alpha;
	
	//get error values for i1
	if(alpha1 > 0 && alpha1 < C) e1 = l_data[i1].error;
	else						 e1 = smo_function(sample, matrix, head, i1) - y1;
	
	//get error values i2
	if(alpha2 > 0 && alpha2 < C) e2 = l_data[i2].error;
	else 						 e2 = smo_function(sample, matrix, head, i2) - y2;
	
	//calculate s
	s = y1*y2;
	
	//compute min and max
	if(s == -1){
		min_val = max(0, alpha2 - alpha1);
		max_val = min(C, C + alpha2 - alpha1);
	}else{
		min_val = max(0, alpha2 + alpha1 - C);
		max_val = min(C, alpha1 + alpha2);
	}
	
	if(min_val == max_val){
		if(verbose > 2) cout << "return0 2" << endl;
		return 0;
	}
	
	//compute eta
	eta = 2.0 * matrix[i1][i2] - matrix[i1][i1] - matrix[i2][i2];
	
	//compute new alpha2
	if(eta < 0){
		new_alpha2 = alpha2 + y2 * (e2 - e1)/eta;
		
			if(new_alpha2 < min_val) new_alpha2 = min_val;
		else if(new_alpha2 > max_val) new_alpha2 = max_val;
	}else{
		//Computing min and max functions
		double c1 = eta/2.0;
		double c2 = y2 * (e1 - e2) - eta * alpha2;
		min_val_f = c1 * min_val * min_val + c2 * min_val;
		max_val_f = c1 * max_val * max_val + c2 * min_val;
			
			if(min_val_f > max_val_f + EPS) new_alpha2 = min_val;
		else if(min_val_f < max_val_f - EPS) new_alpha2 = max_val;
		else new_alpha2 = alpha2;
	}
	
	//exit if no change made
	if(fabs(new_alpha2 - alpha2) < EPS * (new_alpha2 + alpha2 + EPS)){
		if(verbose > 2) cout << "return0 3" << endl;
		return 0;
	}
	
	//calculate new alpha1
	new_alpha1 = alpha1 - s*(new_alpha2 - alpha2);
	if(new_alpha1 < 0){
		new_alpha2 += s * new_alpha1;
		new_alpha1 = 0;
	}else if(new_alpha1 > C){
		new_alpha2 += s * (new_alpha1 - C);
		new_alpha1 = C;
	}
	
	//saving new alphas
	sample->points[i1]->alpha = new_alpha1;
	sample->points[i2]->alpha = new_alpha2;
	
	//saving new stuff into sv list
	if(new_alpha1 > 0 && l_data[i1].sv.empty()){
		head.push_front(i1);
		l_data[i1].sv = head;
	}else if(new_alpha1 == 0 && !l_data[i1].sv.empty()){
		l_data[i1].sv.erase(l_data[i1].sv.begin());
	}
	
	if(new_alpha2 > 0 && l_data[i2].sv.empty()){
		head.push_front(i2);
		l_data[i2].sv = head;
	}else if(new_alpha2 == 0 && !l_data[i2].sv.empty()){
		l_data[i2].sv.erase(l_data[i2].sv.begin());
	}
	
	//update bias
	t1 = y1 * (new_alpha1 - alpha1);
	t2 = y2 * (new_alpha2 - alpha2);
	
	if(new_alpha1 > 0 && new_alpha1 < C)
		bnew = b + e1 + t1 * matrix[i1][i1] + t2 * matrix[i1][i2];
	else{
		if(new_alpha2 > 0 && new_alpha2 < C)
		 bnew = b + e2 + t1 * matrix[i1][i2] + t2 * matrix[i2][i2];
		 else{
		 	double b1 = 0.0, b2 = 0.0;
		 	
		 	b2 = b + e1 + t1 * matrix[i1][i1] + t2 * matrix[i1][i2];
		 	b1 = b + e2 + t1 * matrix[i1][i2] + t2 * matrix[i2][i2];
		 	bnew = (b1 + b2) / 2.0;
		 }
	}
	
	b = bnew;
	sample->bias = -b;
	
	//updating error cache
	error_tot = 0;
	
	for(itr = head.begin(); itr != head.end(); ++itr){
		i = *(itr);
		
		if((i != i1 && i != i2) && sample->points[i]->alpha < C){
			l_data[i].error = smo_function(sample, matrix, head, i) - sample->points[i]->y;
			error_tot += l_data[i].error;
		}
	}
	
	l_data[i1].error = 0.0;
	l_data[i2].error = 0.0;
	
	if(verbose > 1){
		cout << "Total error=" << error_tot << ", alpha(" << i1 << ")= " << new_alpha1 << ", alpha(" << i2 << ")= " << new_alpha2 << endl;
	}
	
	return 1;	
}

/*----------------------------------------------------------*
 * Prints the result of training                            *
 *----------------------------------------------------------*/

void smo_test_learning(Sample *sample, double **matrix, smo_learning_data *l_data, list<int> head){
	int i = 0;
	
	for(i = 0; i < sample->size; ++i){
		cout << i + 1 << " -> " << smo_function(sample, matrix, head, i) << " (error=" << l_data[i].error << ") (alpha=" <<  sample->points[i]->alpha << ")" << endl;
	}
}

/*----------------------------------------------------------*
 * Returns function evaluation at point "index"             *
 *----------------------------------------------------------*/

double smo_function(Sample *sample, double **matrix, list<int> head, int index){
	int i = 0;
	double sum = 0.0;
	list<int>::iterator itr = head.begin();
	cout << head.size() << endl;
	for(itr = head.begin(); itr != head.end(); ++itr){
		i = *(itr);
		
		if(sample->points[i]->alpha > 0){
			sum += sample->points[i]->alpha * sample->points[i]->y * matrix[i][index];
		}
	}
	
	sum += sample->bias;
	
	return sum;
}

/*----------------------------------------------------------*
 * Training function                                        *
 *----------------------------------------------------------*/

int smo_train_matrix(Sample *sample, double **matrix, double &margin, int &svs, int verbose){
	int i = 0, ret = 1;
	double norm = 1;
	smo_learning_data *l_data = NULL;
	list<int> head;
	
	//Allocating array for l_data
	l_data = new smo_learning_data[sample->size];
	if(!l_data) return 0;
	
	//clear data
	sample->bias = 0;
	for(i = 0; i < sample->size; ++i)
		sample->points[i]->alpha = 0;
	
	//run training algorithm
	ret = smo_training_routine(sample, l_data, matrix, head, verbose);
	
	norm = kernel_feature_space_norm(sample, matrix);
	margin = 1.0/norm;
	
	svs = 0;
	for(i = 0; i < sample->size; ++i){
		if(sample->points[i]->alpha > 0) ++svs;
		if(sample->points[i]->alpha > C) ret = 0;
	}
	
	head.clear();
	delete [] l_data;
	
	return ret;
}

/*----------------------------------------------------------*
 * Training function                                        *
 *----------------------------------------------------------*/
 
int smo_train(Sample *sample, double **w, double &margin, int &svs, int verbose){
	int i = 0, ret = 1, size = sample->size;
	double **matrix = NULL;
	double norm = 1.0;
	smo_learning_data *l_data = NULL;
	list<int> head;
	double *w_saved = NULL;
	
	kernel_type = sample->kernel_type;
	kernel_param = sample->kernel_param;
	
	//Allocating array for l_data
	l_data = new smo_learning_data[size];
	if(!l_data){ cout << "l_data - Out of memory!" << endl; return 0; }
	
	//clear data 
	sample->bias = 0;
	for(i = 0; i < size; ++i)
		sample->points[i]->alpha = 0;
		
	//run training algorithm
	matrix = kernel_generate_matrix(sample);
	kernel_print_matrix(matrix, sample->size);
	ret = smo_training_routine(sample, l_data, matrix, head, verbose);
	
	norm = kernel_feature_space_norm(sample, matrix);
	
	if(kernel_type == 0)
		w_saved = utils_get_weight(sample);
	else{
		if(kernel_type == 1 && kernel_param == 1)
			w_saved = utils_get_dualweight_prodint(sample);
		else
			w_saved = utils_get_dualweight(sample); 
	}
	
	*w = w_saved;
	margin = 1.0/norm;
	
	svs = 0;
	for(i = 0; i < size; ++i){
		if(sample->points[i]->alpha > 0) ++svs;
		if(sample->points[i]->alpha > C) ret = 0;
	}
	
	if(verbose){
		cout << "Number of support vectors: " << svs << endl;
		cout << "Margin found: " << margin << "\n\n";
		
		if(verbose > 1){
			for(i = 0; i < sample->dim; ++ i)
				cout << "w[" << sample->fnames[i] << "]: " << w_saved[i] << endl;
			cout << "Bias: " << sample->bias << "\n\n";			
		}
	}
	
	for(i = 0; i < size; ++i){
		delete [] matrix[i];
	}
	delete [] l_data;
	
	head.clear();
	ret = 1;
	
	return ret;
}

int smo_training_routine(Sample *sample, smo_learning_data* l_data, double** matrix, list<int> head,int verbose){
	int epoch = 0, k = 0, num_changed = 0, tot_changed = 0, examine_all = 1, size = sample->size;
	
	//Initialize variables
	sample->bias = 0;
	for(k = 0; k < size; ++k){
		sample->points[k]->alpha = 0;
		l_data[k].error = 0;
		l_data[k].done = 0;
	}
	
	//training
	while(num_changed > 0 || examine_all){
		//stop if iterated too much
		if(epoch > MAX_EPOCH) return 0;

        num_changed = 0;
        if(examine_all)
            for(k = 0; k < sample->size; ++k)
                num_changed += smo_examine_example(sample, l_data, matrix, head, k, verbose);
        else
            for(k = 0; k < sample->size; ++k)
                if(sample->points[k]->alpha > 0 && sample->points[k]->alpha < C)
                    num_changed += smo_examine_example(sample, l_data, matrix, head, k, verbose);

             if(examine_all == 1) examine_all = 0;
        else if(num_changed == 0) examine_all = 1;
        tot_changed += num_changed;
        ++epoch;
	}
	
	//final verbose
	if(verbose){
		smo_test_learning(sample, matrix, l_data, head);
	}
	
	return 1;
}
