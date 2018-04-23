/*********************************************************************************
 * almap.c: ALMA: Approximate Large Margin Algorithm
 * Claudio Gentile. A new approximate maximal margin classification algorithm.
 * JMLR, 2:213-242, 2001.
 *
 * Saulo Moraes Villela <saulomv@gmail.com>
 * Copyright (C) 2013
 *
 *********************************************************************************/

#include <iostream>
#include <cmath>
#include <ctime>
#include <sys/time.h>
#include <cfloat>
#include "data.h"
#include "perceptron.h"
#include "almap.h"

#define MAX_IT 1E9

double START_TIME;

int almap(Sample *sample, double **w, double& margin, int& svs, int verbose){
	double gamma, secs;
	double *w_saved = NULL;
	double bias = 0;
	int *index;
	vector<Point*> points;
	train_data data;
	double rmargin = margin;
	
	data.set_samples(sample);
	if(!sample->normalized){
		data.normalize_dataset();
		sample = data.get_samples();
	}
	
	int i, j, t, c, e, k;
	int size = sample->size, dim = sample->dim;
	double min = 0, max = 0, norm = 0, *func;

	int idx, y, sign;
	vector<double> x;
	double sumnorm = 0.0;

	double p 		  = sample->p;
	double q 		  = p/(p - 1.0);
	sample->q 		  = q;
	double alpha_prox = sample->alpha_aprox;

	double rate 	= 1.0;
	double B 		= 1.0/alpha_prox;
	double C 		= sqrt(2.0);

	//Initializing data struct
	data.norm = 0.0;
	data.func = NULL;
	data.w 	  = NULL;
	data.z    = sample;

	//Allocating space fow w
	if(!(*w)){
		data.w = new double[dim];
		if(!data.w){ cout << "Error: Out of memory\n"; return -1; }
		for(i = 0; i < dim; ++i) data.w[i] = 0.0;
	}else{
		data.w = *w;
		data.norm = 0.0;
		for(i = 0; i < dim; ++i) data.norm += pow(fabs(data.w[i]), q);
		data.norm = pow(data.norm, 1.0/q);
		for(i = 0; i < dim; ++i) data.w[i] /= data.norm;
		data.norm = 1;
	}

	//Allocating space for index and initializing
	if(!sample->index){
		sample->index = new int[size];
		if(!sample->index){ cout << "Error: Out of memory\n"; return -1; }
		for(i = 0; i < size; ++i) sample->index[i] = i;
	}
	sample->bias = 0;

	//Allocating space for w_saved and func
	w_saved = new double[dim];
	if(!w_saved){ cout << "Error: Out of memory\n"; return -1; }
	data.func = new double[size];
	if(!data.func){ cout << "Error: Out of memory\n"; return -1; }

	//Initializing w_saved and func
	for(i = 0; i < dim; ++i) w_saved[i] = 0.0;
	for(i = 0; i < size; ++i) data.func[i] = 0.0;

	func = data.func;
	norm = data.norm;
	bias = data.z->bias = 0;
	points = data.z->points;
	index = data.z->index;

	if(verbose){
		cout << "----------------------------------------------------------------------\n";
		cout << " steps	updates		margin		norm		secs\n";
		cout << "----------------------------------------------------------------------\n";
	}

	t = 0; c = 0; k = 1;
	START_TIME = 100.0f*clock()/CLOCKS_PER_SEC;

	while(t < MAX_IT){
		for(e = 0, i = 0; i < size; ++i){
			idx = index[i];
            x = points[idx]->x;
            y = points[idx]->y;

            gamma = B * sqrt(p-1.0) * (1.0/sqrt(k));

            //calculating function
            for(func[idx] = bias, j = 0; j < dim; ++j)
                func[idx] += data.w[j] * x[j];

			//checking if the point is a mistake
			if(y*func[idx] <= (1.0-alpha_prox)*gamma){
                rate = C / sqrt(p-1.0) * (1.0/sqrt(k));
                for(sumnorm = 0, j = 0; j < dim; ++j){
                    sign = (data.w[j] >= 0) ? 1.0 : -1.0;
                    data.w[j] = sign * pow(fabs(data.w[j]), q-1.0) / pow(norm, q-2.0);
                    data.w[j] += rate * y * x[j];
                    sumnorm += pow(fabs(data.w[j]), p);
                }
                norm = pow(sumnorm, 1.0/p);

                for(sumnorm = 0, j = 0; j < dim; ++j){
                    sign = (data.w[j] >= 0) ? 1.0 : -1.0;
                    data.w[j] = sign * pow(fabs(data.w[j]), p-1.0) / pow(norm, p-2.0);
                    sumnorm += pow(fabs(data.w[j]), q);
                }
                norm = pow(sumnorm, 1.0/q);

                if(norm > 1){
                    for(j = 0; j < dim; ++j)
                        data.w[j] /= norm;
                }
                c++; e++; k++;
            }
        }
        t++; //Number of iterations update

		//stop criterion
		 if(e == 0 && norm > 0){
            //Finding minimum and maximum functional values
            for(min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i)
            {
                     if(func[i] >= 0 && min > func[i]/norm) min = func[i]/norm;
                else if(func[i] <  0 && max < func[i]/norm) max = func[i]/norm;
            }
            //Saving good weights
            for(i = 0; i < dim; i++) w_saved[i] = data.w[i];

            //Obtaining real margin
            rmargin = (fabs(min) > fabs(max)) ? fabs(max) : fabs(min);

            secs = (100.0f*clock()/CLOCKS_PER_SEC-START_TIME)/100.0f;
            if(verbose) cout << " " << t << "	" << c << "		" << rmargin << "		" << norm << "		" << secs << endl;
            break;
        }
	}

	*w = w_saved;
	margin = rmargin;
	sample->margin = margin;
	sample->norm = norm;
	sample->bias = bias;

	if(verbose){
		cout << "---------------------------------------------------------------\n";
		cout << "Number of steps through data: " << t << endl;
		cout << "Number of updates: " << c << endl;
		cout << "Margin found: " << rmargin << endl;
		cout << "\nMin: " << fabs(min) << " / Max: " << fabs(max) << endl;

		if(verbose > 1){
			for(i = 0; i < dim; ++i){
				cout << "W[" << sample->fnames[i] << "]: " << w_saved[i] << endl;
			}
			cout << "Bias: " << sample->bias << endl;
			cout << endl;
		}
	}

	//Freeing stuff
	delete [] data.func;
	delete [] data.w;
	if(!t){
		if(verbose) cout << "ALMAp convergency wasn't reached!\n";
		return 0;
	}
	return 1;
}
