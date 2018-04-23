/*********************************************************************************
 * adaboostdual.c: AdaBoost Primal
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
#include <vector>
#include "data.h"
#include "utils.h"
//#include "cperc.h"
#include "adaboost.h"
#include "perceptron.h"

using namespace std;

void create_set(std::vector<double> dist, int **matrix, int line){
	int i, j, size = dist.size();
	double raffle;
	vector<double> vetProb(size);
	
	vetProb[0] = 0.0;
    for(i = 1; i < size; i++){
        vetProb[i] = vetProb[i-1] + dist[i-1];
    }
    
    for(i = 0; i < size; i++){
        raffle = (double)rand()/((double)RAND_MAX+1);
        for(j = 0; j < size; j++)
            if(vetProb[j] > raffle)
                break;
        matrix[line][i] = j-1;
    }
}

int adaboost(Sample *sample, double ***w, vector<double> margin, vector<double> bias, vector<double> alfa, int verbose){
	int i, j, k, y;
	int cont, it, ctot, passes;
	int size = sample->size, dim = sample->dim, times = sample->timesPerceptron;
	int **matrix = NULL;
	vector<double> x, gamma(times), biastemp(times), distribution(size), alfat(times), beta(times);
	double **w_saved = NULL, *func = NULL;
	train_data data;
	double q = sample->q, max_time = sample->max_time;
	double secs = 0.0, rmargin = 0.0;
	double min = 0.0, max = 0.0, norm = 1.0, sumDistributions = 0.0, perceptronError = 0.0, rate = 0.05;
	
	srand(time(NULL));
	
	matrix = new int*[size];
	if(!matrix) { cout << "Error: Out of memory\n"; return -1;}
	for(k = 0; k < times; ++k){
		matrix[k] = new int[size];
		if(!matrix[k]) { cout << "Error: Out of memory\n"; return -1;}
	}
	
	data.norm	= 1.0;
	data.func 	= NULL;
	data.w		= NULL;
	data.z		= sample;
	
	data.w = new double[dim];
	if(!data.w) { cout << "Error: Out of memory\n"; return -1;}
	data.func = new double[size];
	if(!data.func) { cout << "Error: Out of memory\n"; return -1;}
	w_saved = new double*[times];
	if(!w_saved) { cout << "Error: Out of memory\n"; return -1;}
		
	func = data.func;
	
	for(i = 0; i < times; ++i){
		gamma[i] = 0.0;
		w_saved[i] = new double[dim];
		if(!w_saved[i]) { cout << "Error: Out of memory\n"; return -1;}
		
		for(j = 0; j < dim; ++j) w_saved[i][j] = 0.0;
	}
	
	if(verbose){
		cout << "----------------------------------------------------------------------\n";
		cout << " per	steps	updates	margin	norm	segs\n";
		cout << "----------------------------------------------------------------------\n";	
	}
	
	for(i = 0; i < size; ++i) distribution[i] = 1.0 / size;
	
	for(cont = 0, it = 0; it < times; cont++, it++){
		ctot = 0;
        passes = 0;
        gamma[it] = 0.02;
        double start_time = 100.0f*clock()/CLOCKS_PER_SEC;
        for(i = 0; i < size; ++i) func[i] = 0.0;

        //Random Weight
        for(i = 0; i < dim; ++i){
            data.w[i] = (double)rand()/((double)RAND_MAX+1);
            if (rand()%2 == 1) data.w[i] = -data.w[i];
        }
        data.z->bias = 0.0;

        create_set(distribution, matrix, it);

        if(perceptron_primal_norm2(&data, passes, ctot, matrix[it], q, start_time, max_time, rate)){
            norm = data.norm;
            cout << "\n"<< norm << endl;
            for(min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i){
                if(func[i] != 0.0){
                         if(func[i] >= 0 && min > func[i]) min = func[i];
                    else if(func[i] <  0 && max < func[i]) max = func[i];
                }
            }
            rmargin = (fabs(min) > fabs(max)) ? fabs(max) : fabs(min);
            rmargin /= norm;

            for(perceptronError = 0.0, i = 0; i < size; ++i){
                x = sample->points[i]->x;
                y = sample->points[i]->y;
                for(func[i] = data.z->bias, j = 0; j < dim; ++j)
                    func[i] += data.w[j] * x[j];
                if(func[i] * y < 0.0)
                    perceptronError += distribution[i];
            }
            if(perceptronError >= 0.499999 || perceptronError == 0.0){
                it--;
                continue;
            }
            beta[it] = perceptronError / (1 - perceptronError);

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

            for(i = 0; i < dim; i++) w_saved[it][i] = data.w[i];
            biastemp[it] = data.z->bias;
            gamma[it] = rmargin;

            secs = (100.0f*clock()/CLOCKS_PER_SEC-start_time)/100.0f;
			
			if(verbose){
				if(dim <= 4){
					cout << " " << it+1 << "	" << passes << "	" << ctot << "	" << rmargin << "	" << norm << "	" << secs << endl;
					for(i = 0; i < dim; ++i) cout << "W[" << sample->fnames[i] << "]: " << w_saved[it][i] << endl;
				}else
					cout << " " << it+1 << "	" << passes << "	" << ctot << "	" << rmargin << "	" << norm << "	" << secs << endl;
			}
		}else{
			if(verbose) cout << "Convergency wasn't reached for the Perceptron " << it+1 << "!\n";
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
			for(i = 0; i < dim; ++i) cout << "W[" << j << "][" << sample->fnames[i] << "]: " << w_saved[j][i] << endl;
			cout << "B[" << j << "]: " << biastemp[j] << endl;
			cout << "G[" << j << "]: " << gamma[j] << endl; 
		}
	}
	
	for(j = 0; j < times; ++j){
		alfat[j] = 0.5 * log(1.0 / beta[j]);
		if(verbose) cout << "[" << j << "]: Beta = " << beta[j] << " -- Alfa = " << alfat[j] << endl; 
	}
	
	if(verbose) cout << "Cont: " << cont << endl;
	*w = w_saved;
	margin = gamma;
	bias = biastemp;
	alfa = alfat;
	
	//Freeing stuff
	delete [] data.func;
	data.func = NULL;
	delete [] data.w;
	data.w = NULL;
	for(i = 0; i < times; ++i) delete [] matrix[i];
	delete [] matrix;
	matrix = NULL;
	
	return 1;
}

