#include <iostream>
#include <vector>
#include "data.h"

/*----------------------------------------------------------*
 * Returns sample copied without points                     *
 *----------------------------------------------------------*/

Sample* Sample::copy_sample_zero(Sample *samp){
	int i;
	int dim = samp->dim;
	Sample* smout = new Sample;
	
	//Copying data
	smout->index = NULL;
	smout->size = 0;
	smout->dim = samp->dim;
	smout->bias = samp->bias;
	smout->margin = samp->margin;
	smout->norm = samp->norm; 
	smout->p = samp->p;
	smout->q = samp->q;
	smout->max_time = samp->max_time;
	smout->mult_tempo = samp->mult_tempo;
	smout->kernel_param = samp->kernel_param;
	smout->kernel_type = samp->kernel_type;
	smout->flexible = samp->flexible;
	smout->alpha_aprox = samp->alpha_aprox;
	smout->normalized = samp->normalized;
	smout->empty = samp->empty;
	
	//Copying size of comite
	smout->timesPerceptron = samp->timesPerceptron;
	smout->permutation = samp->permutation;
	smout->comite = samp->comite;
	smout->similarity = samp->similarity;
	
	//Allocating space and copying for new fnames
	smout->fnames.resize(dim, 0);
	for(i = 0; i < dim; ++i)
		smout->fnames[i] = samp->fnames[i];
		
	return smout;
}

Sample* Sample::copy(Sample *samp){
	int i, j;
	int size = samp->size, dim = samp->dim;
	Sample* smout = new Sample;
	
	//Copying data
	smout->size = samp->size;
	smout->dim = samp->dim;
	smout->npos_samples = samp->npos_samples;
	smout->nneg_samples = samp->nneg_samples;
	smout->bias = samp->bias;
	smout->margin = samp->margin;
	smout->norm = samp->norm; 
	smout->p = samp->p;
	smout->q = samp->q;
	smout->max_time = samp->max_time;
	smout->mult_tempo = samp->mult_tempo;
	smout->kernel_param = samp->kernel_param;
	smout->kernel_type = samp->kernel_type;
	smout->flexible = samp->flexible;
	smout->alpha_aprox = samp->alpha_aprox;
	smout->normalized = samp->normalized;
	smout->empty = samp->empty;
	
	//Copying size of comite
	smout->timesPerceptron = samp->timesPerceptron;
	smout->permutation = samp->permutation;
	smout->comite = samp->comite;
	smout->similarity = samp->similarity;
	
	//Allocating space for index array (if exists)
	if(samp->index){
		smout->index = new int[size];
		for(int i = 0; i < size; ++i){
			smout->index[i] = samp->index[i];
		} 
	}else smout->index = NULL;
	
	//Allocating space for new fnames
	smout->fnames.resize(dim, 0);
	for(i = 0; i < dim; ++i)
		smout->fnames[i] = samp->fnames[i];
	
	//Allocating space for and copying information to new data array 
	smout->points.resize(size);
	for(i = 0; i < size; ++i){
		//Initializing
		smout->points[i] = new Point;
		smout->points[i]->y = samp->points[i]->y;
		smout->points[i]->alpha = samp->points[i]->alpha;
		
		//Allocating space and copying features
		smout->points[i]->x.resize(dim, 0.0);
		for(j = 0; j < dim; ++j)
			smout->points[i]->x[j] = samp->points[i]->x[j];
	}
	
	return smout;
}

Sample::~Sample(){
	if(index){
		delete [] index;
	}
	for(vector<Point*>::iterator it = points.begin() ; it != points.end(); ++it){
		delete (*it);
	}
	points.clear(); 
}
