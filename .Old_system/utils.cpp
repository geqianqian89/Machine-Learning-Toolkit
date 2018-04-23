/*****************************************************
 * Utils lib                                         *
 *                                                   *
 * Saul Leite <lsaul@lncc.br>                        *
 * Saulo Moraes <saulomv@gmail.com>                  *
 * 2004 / 2011                                       *
 *****************************************************/

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <ctime>
#include <random>
#include <functional>
#include <sys/time.h>
#include <unistd.h>
#include "data.h"
#include "kernel.h"
#include "utils.h"
//#include "cperc.h"
//#include "cpercdual.h"
//#include "adaboost.h"
//#include "adaboostdual.h"

extern double kernel_param;
extern int kernel_type;

using namespace std;

/*----------------------------------------------------------*
 * Returns weight on feature space with dot product kernel  *
 *----------------------------------------------------------*/

double* utils_get_weight(Sample *sample){
	int i = 0, j = 0, size = sample->size, dim = sample->dim;

	double* w = new double[dim];
	if(!w){
		cerr << "Error: Out of memory\n";
		exit(1);
	}

	for(j = 0; j < dim; ++j){
		for(w[j] = 0, i = 0; i < size; ++i){
			w[j] += sample->points[i]->alpha * sample->points[i]->y * sample->points[i]->x[j];
		}
	}

	return w;
}

/*----------------------------------------------------------*
 * Returns weight on feature space with others kernels      *
 *----------------------------------------------------------*/

double* utils_get_dualweight(Sample *sample){
	int i = 0, j = 0, k = 0;
	int size = sample->size, dim = sample->dim;
	double **H = NULL, **Hk = NULL, **matrixdif = NULL;

	H = kernel_generate_matrix_H(sample);

	double *w = new double[dim];
	if(!w) { cerr << "Error: Out of memory\n"; exit(1); }

	matrixdif = new double*[size];
	if(!matrixdif) { cerr << "Error: Out of memory\n"; exit(1); }
	for(i = 0; i < size; ++i){
		matrixdif[i] = new double[size];
		if(!matrixdif[i]) { cerr << "Error: Out of memory\n"; exit(1); }
	}

	double *alphaaux = new double[size];
	if(!alphaaux) { cerr << "Error: Out of memory\n"; exit(1); }

	for(k = 0; k < dim; ++k){
		Hk = kernel_generate_matrix_H_without_dim(sample, k);

		for(i = 0; i < size; ++i){
			for(j = 0; j < size; ++j){
				matrixdif[i][j] = H[i][j] - Hk[i][j];
			}
		}

		for(i = 0; i < size; ++i){
			for(alphaaux[i] = 0, j = 0; j < size; ++j)
				alphaaux[i] += sample->points[j]->alpha * matrixdif[i][j];
		}

		for(w[k] = 0, i = 0; i < size; ++i)
			w[k] += alphaaux[i] * sample->points[i]->alpha;

		for(i = 0; i < size; ++i){
			delete Hk[i];
		}
		delete Hk;
		Hk = NULL;
	}
	for(i = 0; i < size; ++i){
		delete H[i];
		delete matrixdif[i];
	}
	delete matrixdif;
	delete H;
	delete [] alphaaux;

	return w;
}

/*----------------------------------------------------------*
 * Returns weight on feature space with dot product kernel  *
 *----------------------------------------------------------*/

double* utils_get_dualweight_prodint(Sample *sample){
	int i = 0, j = 0, k = 0;
	int size = sample->size, dim = sample->dim;

	double *w = new double[dim];
	if(!w) { cerr << "Error: Out of memory\n"; exit(1); }

	double *alphaaux = new double[size];
	if(!alphaaux) { cerr << "Error: Out of memory\n"; exit(1); }

	double** H = new double*[size];
	if(!H) { cerr << "Error: Out of memory\n"; exit(1); }
	for(i = 0; i < size; ++i){
		H[i] = new double[size];
		if(!H[i]) { cerr << "Error: Out of memory\n"; exit(1); }
	}

	for(k = 0; k < dim; ++k){
        for(i = 0; i < size; ++i)
            for(j = 0; j < size; ++j)
                H[i][j] = sample->points[i]->x[k] * sample->points[j]->x[k] * sample->points[i]->y * sample->points[j]->y;

        for(i = 0; i < size; ++i)
            for(alphaaux[i] = 0, j = 0; j < size; ++j)
                alphaaux[i] += sample->points[j]->alpha * H[i][j];

        for(w[k] = 0, i = 0; i < size; ++i)
            w[k] += alphaaux[i] * sample->points[i]->alpha;
    }

	for(i = 0; i < size; ++i){
		delete H[i];
	}
	delete H;
	delete [] alphaaux;

	return w;
}

/*----------------------------------------------------------*
 * Returns the norm of a given vector                       *
 *----------------------------------------------------------*/

double utils_norm(vector<double> v, double q){
	double sumnorm = 0.0;
	int i = 0, size = v.size();

	for(i = 0; i < size; ++i){
		sumnorm += pow(fabs(v[i]), q);
	}

	return pow(sumnorm, 1.0/q);
}

/*----------------------------------------------------------*
 * Calculates Leave One Out Error Estimate                  *
 *----------------------------------------------------------*/

double utils_leave_one_out(Sample *sample, int (*train)(Sample*, double**, double&, int&, int), int skip, int verbose){
	int i = 0, j = 0, k = 0, error = 0, svcount = 0;
	double **matrix = NULL;
	Sample *tsample = NULL;
	double *w = NULL;
	double margin = 0.0, func = 0.0;

	matrix = kernel_generate_matrix(sample);

	//Start leave one out
	for(i = 0; i < sample->size; ++i){
		if(skip == 1 && sample->points[i]->alpha == 0) continue;

		//Creating temporary data array
		tsample = Data::remove_point(sample, i);

		//training
		if(!train(tsample, &w, margin, svcount, verbose)){
			if(verbose) cerr << "LeaveOneOut error: On sample " << i << " out, convergence was not achived!\n";
		}
		delete [] w;

		//testing
		func = tsample->bias;
		for(k = 0; k < tsample->size; ++k){
			j = (k >= i) ? k+1 : k;
			func += tsample->points[k]->alpha * tsample->points[k]->y * matrix[j][i];
		}

		if(sample->points[i]->y * func < 0){
			if(verbose) cout << "[" << i << "x]";
			error++;
		}else if(verbose){
			cout << "[" << i << " ]";
			cout.flush();
		}

		if(verbose > 1) cout << "Sample " << i << " (" << sample->points[i]->y << "), function=" << func << endl;

		//freeing data
		delete tsample;
		tsample = NULL;
	}
	if(verbose) cout << endl;

	//free stuff
	for(i = 0; i < sample->size; ++i)
		delete [] matrix[i];
	delete matrix;
	matrix = NULL;

	return (((double)error)/((double)sample->size)*100.0f);
}

/*----------------------------------------------------------*
 * Calculates the error with a K-Fold Crosss-Validation     *
 *----------------------------------------------------------*/

double utils_k_fold(Sample *sample, int (*train)(Sample*, double**, double&, int&, int), int fold, int seed, int verbose){
	int i = 0, j = 0, k = 0;
	double error = 0.0, func = 0.0, margin = 0.0;
	int qtdpos = 0, qtdneg = 0, cost_pos = 0, cost_neg = 0, svcount = 0;
	int *error_arr = NULL;
	Sample *sample_pos = NULL, *sample_neg = NULL;
	Sample *train_sample = NULL, *test_sample = NULL, *traintest_sample = NULL;
	Sample **vet_sample_pos = NULL, **vet_sample_neg = NULL, **vet_sample_final = NULL;
	double *w = NULL;
	double **matrix = NULL;
	Data data_op;

	kernel_type = sample->kernel_type;
	kernel_param = sample->kernel_param;

	error_arr = new int[fold];
	if(!error_arr){ cout << "Error: Out of memory\n"; exit(1); }

	for(i = 0; i < fold; ++i) error_arr[i] = 0;

	sample_pos = Sample::copy_sample_zero(sample);
	sample_neg = Sample::copy_sample_zero(sample);

	for(i = 0; i < sample->size; ++i){
		if(sample->points[i]->y == 1)
			sample_pos = data_op.insert_point(sample_pos, sample, i);
		else
			sample_neg = data_op.insert_point(sample_neg, sample, i);
	}

	qtdpos = sample_pos->size;
	qtdneg = sample_neg->size;

	if(verbose > 1){
		cout << "\nTotal of points: " << sample->size << endl;
		cout << "Qtd of positive: " << qtdpos << endl;
		cout << "Qtd of negative: " << qtdneg << endl;
	}

	mt19937 mt_rand(seed);
	uniform_int_distribution<int> distribution_pos(0,sample_pos->size-1);
	uniform_int_distribution<int> distribution_neg(0,sample_neg->size-1);
	auto dice = bind (distribution_pos, mt_rand);
	auto dice1 = bind (distribution_neg, mt_rand);

	//randomize
	for(i = 0; i < sample_pos->size; ++i){
		Point *aux;

		j = dice();
		aux = sample_pos->points[i];
		sample_pos->points[i] = sample_pos->points[j];
		sample_pos->points[j] = aux;
	}

	for(i = 0; i < sample_neg->size; ++i){
		Point *aux;

		j = dice1();
		aux = sample_neg->points[i];
		sample_neg->points[i] = sample_neg->points[j];
		sample_neg->points[j] = aux;
	}

	vet_sample_pos = new Sample*[fold];
	vet_sample_neg = new Sample*[fold];
	vet_sample_final = new Sample*[fold];

	for(i = 0; i < fold; ++i){
		vet_sample_pos[i] = Sample::copy_sample_zero(sample);
		vet_sample_neg[i] = Sample::copy_sample_zero(sample);
		vet_sample_final[i] = Sample::copy_sample_zero(sample);
	}

	for(i = 0, j = 0; i < fold - 1; ++i){
		for(; j < ((sample_pos->size)-cost_pos)/(fold-i)+cost_pos; ++j)
			vet_sample_pos[i] = data_op.insert_point(vet_sample_pos[i], sample_pos, j);
		cost_pos = j;
	}

	for(; j < sample_pos->size; ++j){
		vet_sample_pos[i] = data_op.insert_point(vet_sample_pos[i], sample_pos, j);
	}

	for(i = 0, j = 0; i < fold-1; ++i){
		for(; j < ((sample_neg->size)-cost_neg)/(fold-i)+cost_neg; ++j)
			vet_sample_neg[fold-1-i] = data_op.insert_point(vet_sample_neg[fold-1-i], sample_neg, j);
		cost_neg = j;
	}

	for(; j < sample_neg->size; ++j)
		vet_sample_neg[fold-1-i] = data_op.insert_point(vet_sample_neg[fold-1-i], sample_neg, j);

	delete sample_pos;
	delete sample_neg;

	for(i = 0; i < fold; ++i){
		for(j = 0; j < vet_sample_pos[i]->size; ++j)
			vet_sample_final[i] = data_op.insert_point(vet_sample_final[i], vet_sample_pos[i], j);
		for(; j < vet_sample_pos[i]->size + vet_sample_neg[i]->size; ++j)
			vet_sample_final[i] = data_op.insert_point(vet_sample_final[i], vet_sample_neg[i], j - vet_sample_pos[i]->size);
	}

	for(i = 0; i < fold; ++i){
		delete vet_sample_pos[i];
		delete vet_sample_neg[i];
	}

	delete [] vet_sample_pos;
	delete [] vet_sample_neg;

	//Start cross-validation
	for(j = 0; j < fold; ++j){
		test_sample = Sample::copy(vet_sample_final[j]);
		train_sample = Sample::copy_sample_zero(sample);
		//Lembra que o erro está aqui!!!!!
		for(i = 0; i < fold; ++i){
			if(i != j)
				for(k = 0; k < vet_sample_final[i]->size; ++k)
					train_sample = data_op.insert_point(train_sample, vet_sample_final[i], k);
		}

		if(verbose){
			cout << "\nCross-Validation " << j + 1 << ": \n";
			cout << "Train points: " << train_sample->size << endl;
			cout << "Test points: " << test_sample->size << endl;
		}

		//training
		w = NULL;
		margin = 0;
		svcount = 0;

		if(!train(train_sample, &w, margin, svcount, 0)){
			if(verbose)
				cerr << "Error at " << fold << "-fold: The convergency wasn't reached at the set " << j+1 << "!\n";
			continue;
		}

		if(sample->kernel_type == 9){
			//testing imap
			for(i = 0; i < test_sample->size; ++i){
				for(func = train_sample->bias, k = 0; k < train_sample->dim; ++k)
					func += w[k] * test_sample->points[i]->x[k];

				if(test_sample->points[i]->y * func <= 0){
					if(verbose > 1)
						cerr << "[" << i+1 << "x] function: " << func << ", y: " << test_sample->points[i]->y << endl;
					error_arr[j]++;
				}else{
					if(verbose > 1)
						cerr << "[" << i+1 << "] function: " << func << ", y: " << test_sample->points[i]->y << endl;
				}
			}
		}else{
			//testing imadual and smo
			traintest_sample = data_op.join_samples(test_sample, train_sample);
			matrix = kernel_generate_matrix(traintest_sample);

			for(i = 0; i < test_sample->size; ++i){
				for(func = train_sample->bias, k = 0; k < train_sample->size; ++k)
					func += train_sample->points[k]->alpha * train_sample->points[k]->y * matrix[k+test_sample->size][i];

				if(test_sample->points[i]->y * func <= 0){
					if(verbose > 1)
						cerr << "[" << i+1 << "x] function: " << func << ", y: " << test_sample->points[i]->y << endl;
					error_arr[j]++;
				}else{
					if(verbose > 1)
						cerr << "[" << i+1 << "] function: " << func << ", y: " << test_sample->points[i]->y << endl;
				}
			}
			for(i = 0; i < traintest_sample->size; ++i)
				delete matrix[i];
			delete [] matrix;
		}

		if(verbose) cout << "Error " << j + 1 << ": " << error_arr[j] << " -- " << ((double)error_arr[j]/(double)vet_sample_final[j]->size)*100.0f << "%";
		error += ((double)error_arr[j]/(double)vet_sample_final[j]->size)*100.0f;

		//freeing data
		delete [] w;
		delete train_sample; train_sample = NULL;
		delete test_sample; test_sample = NULL;
		delete traintest_sample; traintest_sample = NULL;
	}

	//freeing data
	for(i = 0; i < fold; ++i)
		delete vet_sample_final[i];
	delete vet_sample_final;
	delete [] error_arr;

	return (((double)error)/(double)fold);
}

/*---------------------------------------------------------------*
 * Compute the validation error, besides of calling the k-fold   *
 *---------------------------------------------------------------*/

void utils_validation(Sample *train_sample, Sample *test_sample, int (*train)(Sample*, double**, double&, int&, int), int fold, int qtde, int verbose){
	int i = 0, k = 0, erro = 0, svcount = 0;
	double errorcross = 0.0, func = 0.0, margin = 0.0;
	double *w = NULL, **matrix = NULL;
	Sample *traintest_sample = NULL;
	Data data_op;

	kernel_type = train_sample->kernel_type;
	kernel_param = train_sample->kernel_param;

	//cross-validation
	if(qtde > 0){
		for(errorcross = 0, i = 0; i < qtde; ++i){
			if(verbose)
				cout << "\n\nExecution " << i+1 << " / " << qtde << ":\n";
			errorcross += utils_k_fold(train_sample, train, fold, i, verbose);
		}
		cout << "\n\nError " << fold << "-Fold Cross Validation: " << errorcross/qtde << "%" << endl;
	}

	//start final validation
	if(verbose){
		cout << "\nFinal Validation:\n";
		cout << "Train points: " << train_sample->size << endl;
		cout << "Test points: " << test_sample->size << endl;
	}

	//training
	if(!train(train_sample, &w, margin, svcount, 0))
		if(verbose)
			cerr << "Validation Error: convergency wasn't reached in the training set!\n";

	if(train_sample->kernel_type == 9){
		//testing imap
		for(i = 0; i < test_sample->size; ++i){
			for(func = train_sample->bias, k = 0; k < train_sample->dim; ++k)
				func += w[k] * test_sample->points[i]->x[k];

			if(test_sample->points[i]->y * func <= 0){
				if(verbose > 1) cerr << "[" << i+1 << "x] function: " << func << ", y: " << test_sample->points[i]->y << endl;
				erro++;
			}else{
				if(verbose > 1) cerr << "[" << i+1 << "] function: " << func << ", y: " << test_sample->points[i]->y << endl;
			}
		}
	}else{
		//testing imadual and smo
		traintest_sample = data_op.join_samples(test_sample, train_sample);
		matrix = kernel_generate_matrix(traintest_sample);

		for(i = 0; i < test_sample->size; ++i){
			for(func = train_sample->bias, k = 0; k < train_sample->size; ++k)
				func += train_sample->points[k]->alpha * train_sample->points[k]->y * matrix[k+test_sample->size][i];

			if(test_sample->points[i]->y * func <= 0){
				if(verbose > 1) cerr << "[" << i+1 << "x] function: " << func << ", y: " << test_sample->points[i]->y << endl;
				erro++;
			}else{
				if(verbose > 1) cerr << "[" << i+1 << "] function: " << func << ", y: " << test_sample->points[i]->y << endl;
			}
		}

		for(i = 0; i < traintest_sample->size; ++i)
			delete matrix[i];
		delete [] matrix;
	}

	cout << "Validation Error: " << erro << " -- " << ((double)erro/(double)test_sample->size)*100.0f << "%";

	//freeing data
	delete [] w;
	w = NULL;
	delete traintest_sample;
}
