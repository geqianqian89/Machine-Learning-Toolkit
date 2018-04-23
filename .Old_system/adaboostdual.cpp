/*********************************************************************************
 * adaboostdual.c: AdaBoost Dual
 *
 * Saulo Moraes Villela <saulo.moraes@ufjf.edu.br>
 * Karen Enes <kakaenes@gmail.com>
 * Copyright (C) 2015
 *
 *********************************************************************************/

#include <iostream>
#include <cmath>
#include <ctime>
#include <cfloat>
#include <sys/time.h>
#include "data.h"
#include "utils.h"
#include "kernel.h"
#include "adaboost.h"
#include "adaboostdual.h"
#include "perceptron.h"

int adaboostdual(Sample *sample, double ***alpha, vector<double> margin, vector<double> bias, vector<double> alfa, Point ****points_temp, int verbose){
	int i, j, k;
	int y, cont, it, ctot, passes;
	int size = sample->size, dim = sample->dim;
	double min = 0.0, max = 0.0, norm = 1.0, *func = NULL,  secs = 0.0, rmargin = 0.0;
    double max_time = sample->max_time;
    int times = sample->timesPerceptron;
    int **matrix = NULL;
    double sumDistributions = 0.0, perceptronError = 0.0, rate = 0.05;
	vector<double> gamma(times), biastemp(times), alfat(times), beta(times), distribution(size);
    Sample *sample_temp = NULL;
	Point ***point_t = NULL;
	datadual data;
    double **saved_alphas = NULL;

	srand(time(NULL));

	data.norm 	= 1.0;
	data.func 	= NULL;
	data.z 		= sample;
	sample->bias = 0.0;
	
	matrix = new int*[times];
	if(!matrix) { cout << "Error: Out of memory\n"; return -1; }
	for(k = 0; k < times; ++k){
		matrix[k] = new int[size];
		if(!matrix[k]) { cout << "Error: Out of memory\n"; return -1; }
	}

	saved_alphas = new double*[times];
	if(!saved_alphas) { cout << "Error: Out of memory\n"; return -1; }

	for(i = 0; i < times; ++i){
		saved_alphas[i] = new double[size];
		if(!saved_alphas[i]) { cout << "Error: Out of memory\n"; return -1; }
		
		for(j = 0; j < size; ++j) saved_alphas[i][j] = 0.0;
	}
	
	point_t = new Point**[times];
	for(j = 0; j < times; ++j){
		point_t[j] = new Point*[size];
		
		for(i = 0; i < size; ++i){
			point_t[j][i] = new Point;
			if(!point_t[j][i]){ cout << "Error: Out of memory\n"; return -1; }
			point_t[j][i]->x.assign(dim, 0.0);
		}
	}
	
	data.func = new double[size];
	if(!data.func) { cout << "Error: Out of memory\n"; return -1; }
	func = data.func;

	if(verbose){
		cout << "----------------------------------------------------------------------\n";
		cout << " per    steps     updates        margin            norm        secs\n";
		cout << "----------------------------------------------------------------------\n";
	}
	
	data.K = kernel_generate_matrix(sample);
	
	for(cont = 0, it = 0; it < times; ++cont, ++it){
        ctot = 0;
        passes = 0;
        gamma[it] = 0;
        double start_time = 100.0f*clock()/CLOCKS_PER_SEC;
        for(i = 0; i < size; ++i) func[i] = sample->points[i]->alpha = 0.0;

        data.z->bias = 0.0;
        data.norm = 1.0;

        create_set(distribution, matrix, it);

        sample_temp = Sample::copy(sample);
        data.change_x_vector(sample_temp, matrix[it]);

        if(perceptron_dual(&data, passes, ctot, matrix[it], start_time, max_time, rate)){
            norm = data.norm;
            for(min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i){
                if(func[i] != 0.0){
                         if(func[i] >= 0 && min > func[i]) min = func[i];
                    else if(func[i] <  0 && max < func[i]) max = func[i];
                }
            }
            rmargin = (fabs(min) > fabs(max)) ? fabs(max) : fabs(min);
            rmargin /= norm;

            for(perceptronError = 0.0, i = 0; i < size; ++i){
                for(func[i] = data.z->bias, k = 0; k < size; ++k){
                    double kernel = kernel_function(sample_temp->points[k], sample->points[i], dim);
                    func[i] += sample->points[k]->alpha * sample->points[matrix[it][k]]->y * kernel;
                }

                if(func[i] * sample->points[i]->y < 0.0)
                    perceptronError += distribution[i];
            }
            delete sample_temp;
            sample_temp = NULL;

            if(perceptronError >= 0.499999 || perceptronError == 0.0){
                it--;
                continue;
            }
            beta[it] = perceptronError / (1 - perceptronError);

            for(i = 0; i < size; i++){
                for(j = 0; j < dim; ++j)
                    point_t[it][i]->x[j] = sample->points[matrix[it][i]]->x[j];
                point_t[it][i]->y = sample->points[matrix[it][i]]->y;
            }

            for(sumDistributions = 0.0, i = 0; i < size; i++){
                y = sample->points[i]->y;
                if(func[i] * y >= 0.0)
                    distribution[i] *= beta[it];
                sumDistributions += distribution[i];
            }

            for(i = 0; i < size; i++)
                distribution[i] /= sumDistributions;

            for(sumDistributions = 0.0, i = 0; i < size; ++i){
                sumDistributions += distribution[i];
            }

            for(i = 0; i < size; i++) saved_alphas[it][i] = sample->points[i]->alpha;
            biastemp[it] = data.z->bias;
            gamma[it] = rmargin;

            secs = (100.0f*clock()/CLOCKS_PER_SEC-start_time)/100.0f;
			
			if(verbose){
				if(size <= 5){
					cout << " " << it+1 << "   " << passes << "    " << ctot << "    " << rmargin << "    " << norm << "      " << secs << " | ";
					
					for(i = 0; i < size; ++i)
						cout << "A[" << i << "]: " << saved_alphas[it][i] << " | ";
						cout << "Bias: " << biastemp[it] << endl;
				}else{
					cout << " " << it+1 << "   " << passes << "    " << ctot << "    " << rmargin << "    " << norm << "      " << secs << endl;
				}
			}
		}else{
			if(verbose) cout << "Convergency wasn't reached for the Perceptron " << it + 1 << endl;
			it--;
		}
		
		if(verbose > 1){
			cout << "\n----------------------------------------------------------------------\n";
			cout << "Perceptron: " << it+1 << endl;
			cout << "Number of steps through data: " << passes << endl;
			cout << "Number of updates: " << ctot << endl;
			cout << "Margin found: " << rmargin << endl;
			cout << "Min: " << fabs(min) << " / Max: " << fabs(max) << "\n\n";
		}
	}
	
	if(verbose > 1){
		for(j = 0; j < times; ++j){
			for(i = 0; i < size; ++i) cout << "A[" << j << "][" << i << "]: " << saved_alphas[j][i] << " ";
			 cout << "B[" << j << "]: " << biastemp[j] << " ";
			 cout << "G[" << j << "]: " << gamma[j] << endl;
		}
	}
	
	for(j = 0; j < times; ++j){
		alfat[j] = 0.5 * log(1.0 / beta[j]);
		if(verbose) cout << "[" << j << "]: Beta = " << beta[j] << " -- Alfa = " << alfat[j] << endl;
	}
	
	if(verbose) cout << "Cont: " << cont << endl;
	*alpha = saved_alphas;
	*points_temp = point_t;
	margin = gamma;
	bias = biastemp;
	alfa = alfat;
	
	//Freeing stuff
	delete [] data.func;
	data.func = NULL;
	for(k = 0; k < size; ++k){
		delete [] data.K[i];
	}
	data.K[i] = NULL;
	for(k = 0; k < times; ++k){
		delete [] matrix[k];
	}
	delete [] matrix;
	matrix = NULL;	
	
	return 1;
}	

