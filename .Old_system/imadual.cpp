/*********************************************************************************
 * imadual.cpp: IMA algorithm in dual variables
 *
 * Saul Leite <lsaul@lncc.br>
 * Saulo Moraes <saulomv@gmail.com>
 * Copyright (C) 2006/2011
 *
 *********************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cfloat>
#include <iomanip>
#include <limits>
#include "data.h"
#include "kernel.h"
#include "utils.h"
#include "imadual.h"
#include "perceptron.h"

#define MIN_INC	1.001
#define EPS 	1E-9

extern double kernel_param;
extern int kernel_type;

int imadual(Sample *sample, double **w, double& margin, int& svs, int verbose){
	int it, ctot, steps;
	double rmargin = 0, gamma, secs;
	datadual data;
	double *w_saved = NULL;
	double bias = 0;
	double max_time;
	double rate = 1;

	int i, sv = 0, size = sample->size, dim = sample->dim;
	double min, max, norm = 0, *saved_alphas = NULL, *func;
	vector<Point*> points = sample->points;

	max_time 	 = sample->max_time;
	kernel_type  = sample->kernel_type;
	kernel_param = sample->kernel_param;

	//Initializing data struct
	data.norm = 0.0;
	data.func = NULL;
	data.z 	  = sample;

	//Allocating space for index
	if(!sample->index){
		sample->index = new int[size];
		if(!sample->index){ cout << "Error: Out of memory\n"; return -1; }

		//Initiializing alpha and bias
		for(i = 0; i < size; ++i){
			sample->index[i] = i;
			points[i]->alpha = 0.0;
		}
	}
	sample->bias = 0;

	//Allocating space for func
	data.func = new double[size];
	if(!data.func){ cout << "Error: Out of memory\n"; return -1; }

	//Allocating space for saved alphas
	saved_alphas = new double[size];
	if(!saved_alphas){ cout << "Error: Out of memory\n"; return -1; }

	//Initializing func
	for(i = 0; i < size; ++i) data.func[i] = 0.0;
	func = data.func;

	//Allocating space kernel matrix
	data.K = kernel_generate_matrix(sample);

	if(verbose){
		cout << "----------------------------------------------------------------------\n";
		cout << "  steps    updates		margin		norm	sv  	secs\n";
		cout << "----------------------------------------------------------------------\n";
	}

	it = ctot = steps = gamma = 0;

	double start_time = 100.0f*clock()/CLOCKS_PER_SEC;

	while(perceptron_fixed_margin_dual(&data, gamma, steps, ctot, sample->index, start_time, max_time, rate)){
		//Finding minimum and maximum functional values
		norm = data.norm;
		bias = data.z->bias;
		for(sv = 0, min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i){
	        if(points[i]->alpha > EPS*rate) { sv++; saved_alphas[i] = points[i]->alpha; }
            else                           { saved_alphas[i] = 0.0; }
                 if(func[i] >= 0 && min > func[i]/norm) min = func[i]/norm;
            else if(func[i] <  0 && max < func[i]/norm) max = func[i]/norm;
        }

		//Obtaining real margin
        rmargin = (fabs(min) > fabs(max)) ? fabs(max) : fabs(min);

		//Obtaining new gamma_f
        gamma = (min-max)/2.0;
        if(gamma < MIN_INC*rmargin) gamma = MIN_INC*rmargin;

        secs = (100.0f*clock()/CLOCKS_PER_SEC-start_time)/100.0f;

		if(verbose){
			cout << "  " << steps << "    " << ctot << "		" << rmargin << "		" << norm << "	" << sv << "	" << secs << endl;
		}

		++it;	//IMA iteration increment

	}

	for(i = 0; i < size; ++i) points[i]->alpha = saved_alphas[i];

    sample->bias = bias;
    norm = kernel_feature_space_norm(sample, data.K);

    /*recovering the vector DJ -- "weights" of the components*/
    if(kernel_type == 0)
        w_saved = utils_get_weight(sample);
    else{
        if(kernel_type == 1 && kernel_param == 1)
            w_saved = utils_get_dualweight_prodint(sample);
        else{
            w_saved = utils_get_dualweight(sample);
        }
        if(it) Data::normalized(w_saved, dim, 2);
    }

    *w      = w_saved;
    margin = rmargin;
    svs    = sv;

    if(verbose){
    	cout << "----------------------------------------------------------------------\n";
		cout << "Number of times that the Fixed Margin Perceptron was called: " << it + 1 << endl;
		cout << "Number of steps through the data: " << steps << endl;
		cout << "Number of updates: " << ctot << endl;
		cout << "Margin found: " << rmargin << endl;
		cout << "\nMin: " << fabs(min) << " / Max: " << fabs(max) << "\n" << endl;

		if(verbose > 1){
			for(i = 0; i < dim; ++i){
				cout << "W[" << sample->fnames[i] << "]: " << w_saved[i] << endl;
			}
			cout << "Bias: " << sample->bias << "\n" << endl;
		}
    }

    for(i = 0; i < size; ++i){
    	delete [] data.K[i];
    }
    delete [] data.K;
    data.K = NULL;
    delete [] data.func;
    delete [] saved_alphas;

    if(!it){
    	if(verbose) cout << "FMP convergency was not reached!\n";
    	return 0;
    }
    return 1;
}
