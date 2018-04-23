/*********************************************************************************
 * imap.cpp: IMA algorithm in Primal variables with Norm Lp
 *
 * Saul Leite <lsaul@lncc.br>
 * Saulo Moraes <saulomv@gmail.com>
 * Copyright (C) 2006/2011
 *
 *********************************************************************************/

#include <iostream>
#include <cmath>
#include <ctime>
#include <cfloat>
#include <sys/time.h>
#include "data.h"
#include "imap.h"
#include "perceptron.h"

#define MIN_INC  1.001
#define MAX_IT   1E9
#define MAX_UP   1E9
#define EPS      1E-9

int imap(Sample *sample, double **w, double& margin, int& svs, int verbose){
	int it, ctot, steps, j, t1 = 1, t3 = 1, flagNao1aDim = 0;
	int i, size = sample->size, dim = sample->dim;
	long int tMax = 0;
	double gamma, secs, bias = 0.0, rmargin = margin, y, alpha;
	double min = 0.0, max = 0.0, norm = 0.0, *func, RATE = 1.0, inc;
	double q = sample->q, max_time = sample->max_time, flexible = sample->flexible;
	double *w_saved = NULL;
	train_data data;

	//Initializing data struct
	data.norm = 1.0f;
	data.func = NULL;
	data.w 	  = NULL;
	data.z 	  = sample;
	
	//Allocating space for w_saved and func
	w_saved = new double[dim];
	if(!w_saved){ cout << "Error: Out of memory\n"; return -1; }
	data.func = new double[size];
	if(!data.func){ cout << "Error: Out of memory\n"; return -1; }
	func = data.func;
	
	//Allocating space for w
	if(!(*w)){
		data.w = new double[dim];
		if(!data.w){ cout << "Error: Out of memory\n"; return -1; }
		for(i = 0; i < dim; ++i) data.w[i] = 0.0; 
	}else{
		data.w = *w;
        if(q == 1)
            for(data.norm = 0.0, i = 0; i < dim; ++i) data.norm += fabs(data.w[i]);
        else if(q == 2){
            for(data.norm = 0.0, i = 0; i < dim; ++i) data.norm += data.w[i]*data.w[i];
            data.norm = sqrt(data.norm);
        }
        else{
            for(data.norm = 0.0, i = 0; i < dim; ++i) data.norm += pow(fabs(data.w[i]), q);
            data.norm = pow(data.norm, 1.0/q);
        }
        for(i = 0; i < dim; ++i) data.w[i] /= data.norm;
        data.z->bias /= data.norm;
        data.norm = 1;
        flagNao1aDim = 1;
        int flag = 0;
        for(min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i){
            y = data.z->points[i]->y;
            for(func[i] = 0, j = 0; j < dim; ++j)
                func[i] += data.w[j] * data.z->points[i]->x[j];
                 if(y ==  1 && func[i] < min) min = func[i];
            else if(y == -1 && func[i] > max) max = func[i];
        }
        
        data.z->bias = - (min + max) / 2.0;
		
		for(min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i){
            y = data.z->points[i]->y;
            for(func[i] = data.z->bias, j = 0; j < dim; ++j)
                func[i] += data.w[j] * data.z->points[i]->x[j];
            if(func[i] * y < 0) flag++;
                 if(y ==  1 && func[i] < min) min = func[i];
            else if(y == -1 && func[i] > max) max = func[i];
        }
        if(flag) rmargin = 0;
        else rmargin = fabs(min);
        if(margin == 0) tMax = MAX_UP;
        else{
            double radius = data.get_radius(-1, q);
            tMax = (radius*radius - rmargin*rmargin) / pow(margin - rmargin, 2);
            if(rmargin == 0) tMax *= 1.5;
        }
	}	
	
	//Allocating space for index and initializing
	if(!sample->index){
		sample->index = new int[size];
		if(!sample->index){ cout << "Error: Out of memory\n"; return -1; }
		for(i = 0; i < size; ++i) sample->index[i] = i;
		sample->bias = 0.0;
	}
	
	//Initializing w_saved and func
	for(i = 0; i < dim; ++i) w_saved[i]	= 0.0;
	for(i = 0; i < size; ++i){ 
		data.func[i] = 0.0; 
		data.z->points[i]->alpha = 0.0; 
	}
	
	if(verbose){
		cout << "----------------------------------------------------------------------\n";
		cout << " pmf	steps	updates		margin		norm		secs\n";
		cout << "----------------------------------------------------------------------\n";
	}
	
	it = 0; ctot = 0; steps = 0; gamma = 0.0;
	double START_TIME = 100.0f*clock()/CLOCKS_PER_SEC;

	while(perceptron_fixed_margin_primal(&data, gamma, &steps, &ctot, sample->index, q, START_TIME, max_time, RATE)){
		
		//Finding minimum and maximum functional values
        norm  = data.norm;
        bias  = data.z->bias;
        for(min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i){
            y = data.z->points[i]->y;
            alpha = data.z->points[i]->alpha;
            if((func[i] + y*alpha*flexible) >= 0 && min > (func[i] + y*alpha*flexible)/norm) min = (func[i] + y*alpha*flexible)/norm;
            else if((func[i] + y*alpha*flexible) <  0 && max < (func[i] + y*alpha*flexible)/norm) max = (func[i] + y*alpha*flexible)/norm;
        }

        //Saving good weights
        for(i = 0; i < dim; i++) w_saved[i] = data.w[i];

        //Obtaining real margin
        rmargin = (fabs(min) > fabs(max)) ? fabs(max) : fabs(min);

        //Shift no bias
        double mmargin = (fabs(max) + fabs(min)) / 2.0;
        if(fabs(max) > fabs(min))
            data.z->bias += fabs(mmargin - rmargin);
        else
            data.z->bias -= fabs(mmargin - rmargin);

        //Obtaining new gamma_f
        gamma = (min-max)/2.0;
        inc = (1+sample->alpha_aprox)*rmargin;
        if(gamma < inc) gamma = inc;
        rmargin = mmargin;

        if(it == 2)
            t1 = ctot;
        t3 = ctot;

        if(it > 1){
            RATE = sqrt(t1) / sqrt(t3);
            if(verbose) cout << "RATE: "<< RATE << "\n";
        }
        else if(it == 1 && verbose)
           cout << "RATE: "<< RATE << "\n";

        secs = (100.0f*clock()/CLOCKS_PER_SEC-START_TIME)/100.0f;
        if(verbose) cout << " " << it + 1 << "	" << steps << "	" << ctot << "		" << rmargin << "		" << (double)norm << "		" << secs << endl;

        ++it; //IMA iteration increment

        if(flagNao1aDim) break;
	}
	
	*w = w_saved;
	margin = rmargin;
	sample->margin = rmargin;
	sample->norm = norm;
	sample->bias = bias;
	
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
	
	//Freeing stuff 
	delete [] data.func;
	delete [] data.w;
	
	if(!it)
    {
        if(verbose) cout << "FMP convergency was not reached\n";
        return 0;
    }
    
    return 1;
}

