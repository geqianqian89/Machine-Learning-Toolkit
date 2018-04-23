/*****************************************************
 * recursive feature elimination lib                 *
 *                                                   *
 * Saul Leite <lsaul@lncc.br>                        *
 * sep 23, 2004                                      *
 *****************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "data.h"
#include "utils.h"
#include "rfe.h"

#define first_decay 0.25

/*----------------------------------------------------------*
 * RFE feature selection                                    *
 *----------------------------------------------------------*/

Sample* rfe_select_features(string filename, Sample *sample, int (*train)(Sample*, double**, double&, int&, int), int depth, int jump, int leave_one_out, int skip, crossvalidation *cv, int verbose){
	int i = 0, j = 0, svcount = 0, dim = sample->dim, level = 0;
	int leveljump = 0, partial_dim = 0, partial_svs = 0, partial = 0;
	int *partial_features = NULL;
	double margin = 0.0, leave_oo = 0.0, errorkfold = 0.0,  START_TIME = 100.0f*clock()/CLOCKS_PER_SEC;
	double partial_time = 0.0, partial_margin = 0.0;
	double *w = NULL, *new_w = NULL;
	Data _data;
	Sample* stmp = sample, *partial_stmp = NULL;
	vector<rfe_select_weight> weight;
	vector<int> features;
	
	//error check
	if(depth < 1 || depth >= dim){
		cout << "Invalid depth!\n";
		exit(1);
	}
	
	//creating an array to place selected features
	features.resize(depth);
	
	//initializing cross validation
	if(cv->qtd > 0){
		cv->seed = new int[cv->qtd];
		if(!cv->seed){ cerr << "Out of memory!" << endl; exit(1); }
		for(i = 0; i < cv->qtd; ++i){
			cv->seed[i] = i;
		}
		cv->initial_error = 0;
		cv->actual_error = 0;
	}
	double n0 = 1;
	
	while(true){
		svcount = 0;
		margin = 0;
		
		if(level == 1)
			n0 = stmp->max_time *= first_decay;
		else if(level > 1)
			stmp->max_time = n0 * exp(-stmp->mult_tempo * ((double)dim/(dim - level)));
			
		//training sample
		if(!train(stmp, &w, margin, svcount, 0)){
			delete [] w; w = NULL;
			if(verbose) cout << "Training failed!\n";
			if(level > 0){
				cout << "---------------\n :: END :: \n---------------\n";
				cout << "Choosen Features: ";
				for(i = 0; i < partial_stmp->dim-1; ++i)
					cout << partial_stmp->fnames[i] << ", ";
				cout << partial_stmp->fnames[i] << endl;
				
				if(cv->qtd > 0){
					if((dim - partial_dim) % cv->jump != 0){
						for(cv->actual_error = 0, i = 0; i < cv->qtd; ++i)
							cv->actual_error += utils_k_fold(stmp, train, cv->fold, cv->seed[i], 0);
						errorkfold = cv->actual_error / cv->qtd; 
					}
		
					cout << "Dim: " << partial_dim << ", Margin: " << partial_margin << ", SVs: " << partial_svs << ", Error " << cv->fold << "-fold: " << errorkfold << endl;
				}else
					cout << "Dim: " << partial_dim << ", Margin: " << partial_margin << ", SVs: " << partial_svs << endl;
				cout << "---------------\nTotal time: " << partial_time << "s\n\n";
				partial = 1;
				_data.write(filename, partial_stmp, 0);
			}
			break;
		}
		
		partial_margin = margin;
		partial_svs = svcount;
		partial_time = (100.0f*clock()/CLOCKS_PER_SEC-START_TIME)/100.0f;
		partial_dim = dim - level;
	
		delete partial_stmp;
		partial_stmp = Sample::copy(stmp);
	
		delete [] partial_features;
		if(level-jump > 0){
			partial_features = new int[level-jump];
			if(!partial_features){ cout << "Out of memory! depth = " << depth << endl; exit(1); }
		}
	
		for(i = 0; i < level-jump; ++i)
			partial_features[i] = features[i];
		
		if(cv->qtd > 0){
			if(level == 0){
				for(i = 0; i < cv->qtd; ++i){
					cv->initial_error += utils_k_fold(stmp, train, cv->fold, cv->seed[i], 0);
				}
				errorkfold = cv->actual_error / cv->qtd;
			}else if(level % cv->jump == 0){
				for(cv->actual_error = 0, i = 0; i < cv->qtd; ++i){
					cv->actual_error += utils_k_fold(stmp, train, cv->fold, cv->seed[i], 0);
				}
				errorkfold = cv->actual_error / cv->qtd;
			}
		}
	
		//leave one out
		if(leave_one_out){
			leave_oo = utils_leave_one_out(stmp, train, skip, 0);
			cout << "LeaveOO -- Dim: " << (dim-level)  << ", Margin: " << margin << ", LeaveOO: " << leave_oo << ", SVs: " << svcount << endl;
		}else if(verbose){
			if(cv->qtd > 0 && level % cv->jump == 0)
				cout << "Dim: " << (dim-level) << ", Margin: " << margin << ", SVs: " << svcount << ", Error " << cv->fold << "-fold: " << errorkfold << endl;
			else
				cout << "Dim: " << (dim-level) << ", Margin: " << margin << ", SVs: " << svcount << endl;
		}
	
		weight.resize(stmp->dim);
	
		//copying elements of array
		for(i = 0; i < stmp->dim; ++i){
			weight[i].w = w[i];
			weight[i].fname = stmp->fnames[i];
		}
	
		//sorting
		sort(weight.begin(), weight.end(), rfe_select_compare_weight_greater);
	
		cout << "---------------------\n";
		if(verbose > 1){
			for(i = 0; i < stmp->dim; ++i)
				cout << weight[i].fname << ": " << weight[i].w << endl;
			cout << "---------------------\n";
		}
	
		//stopping criterion
		if(level >= depth || (cv->qtd > 0 && cv->actual_error-cv->initial_error > cv->limit_error)){
			cout << "---------------\n :: END :: \n---------------\n";
			cout << "Choosen Features: ";
			for(i = 0; i < stmp->dim-1; ++i)
				cout << stmp->fnames[i] << ", ";
			cout << stmp->fnames[i] << endl;
		
			cout << "---------------\nEliminated Features: ";
			for(i = 0; i < leveljump-1; ++i)
				cout << features[i] << ", ";
			cout << features[i] << endl;
		
			if(cv->qtd > 0){
				if(level % cv->jump != 0){
					for(cv->actual_error = 0, i = 0; i < cv->qtd; ++i)
						cv->actual_error += utils_k_fold(stmp, train, cv->fold, cv->seed[i], 0);
					errorkfold = cv->actual_error / cv->qtd;
				}
				cout << "Dim: " << dim-level << ", Margin: " << margin << ", SVs: " << svcount << ", Error " << cv->fold << "-fold: " << errorkfold << endl;
			}else{
				cout << "Dim: " << (dim-level) << ", Margin: " << margin << ", SVs: " << svcount << endl;
			}
		
			cout << "---------------\nTotal time: " << (100.0f*clock()/CLOCKS_PER_SEC-START_TIME)/100.0f << "s\n\n";
			_data.write(filename, stmp, 0);
			weight.clear();
			break;
		}
	
		if(level+jump > depth)
			leveljump = depth;
		else
			leveljump = level+jump;
		
		//maintenance of the w of the dad for the IMA Primal
		if(stmp->kernel_type == 9){
			for(j = 0; j < stmp->dim; ++j){
				for(i = level; i < leveljump; ++i)
					if(weight[i-level].w == w[j])
						w[j] = 0;
			}
			
			new_w = new double[sample->dim-leveljump];
			if(!new_w){ cout << "Error: Out of memory\n"; exit(1); }
		
			for(i = 0, j = 0; j < stmp->dim; ++j){
				if(w[j] != 0)
					new_w[i++] = w[j];
			}
			delete [] w;
			w = new_w;
			new_w = NULL;
		}else{	//IMA Dual and SMO
			delete [] w;
			w = NULL;
		}
		
		if(stmp != sample) delete sample;
		
		//saving removed feature name
		for(i = level; i < leveljump; ++i){
			cout << "Removing w = " << weight[i-level].w << endl;
			features[i] = weight[i-level].fname;
		}
		cout << "---------------------\n";
		weight.clear();
		
		//increment
		if(level+jump > depth){
			level = depth;
			jump = 0;
		}else
			level += jump;
			
		//get temp data struct
		stmp = Data::remove_features(sample, features);
	}
	
	features.clear();
	delete [] partial_features;
	
	if(cv->qtd > 0) delete [] cv->seed;
	
	delete [] w;
	delete sample;
	
	if(partial){
		delete stmp;
		return partial_stmp;
	}else{
		delete partial_stmp;
		return stmp;
	}
}

/*----------------------------------------------------------*
 * Returns 1 for a > b, -1 a < b, 0 if a = b                *
 *----------------------------------------------------------*/

bool rfe_select_compare_weight_greater(rfe_select_weight a, rfe_select_weight b){
	return (fabs(a.w) < fabs(b.w));
}
