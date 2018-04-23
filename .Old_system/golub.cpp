/*****************************************************
 * golub feature selection                           *
 *                                                   *
 * Saul Leite <lsaul@lncc.br>                        *
 * sep 23, 2004                                      *
 *****************************************************/
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
#include "data.h"
#include "utils.h"
#include "golub.h"

Sample* golub_select_features(string filename, Sample *sample, int (*train)(Sample*, double**, double&, int&, int), int number, int verbose){
	int i = 0, j = 0, num_pos = 0, num_neg = 0, dim = sample->dim, size = sample->size, partial = 0;
	int svs = 0;
	double *avg_neg = NULL, *avg_pos = NULL, *sd_neg = NULL, *sd_pos = NULL;
	double *w = NULL, margin = 0.0; 
	vector<golub_select_score> scores;
	vector<int> remove;
	Data data;
	Sample *stmp = NULL, *stmp_partial = NULL;
	
	//Alloc memory
	avg_pos = new double[dim];
	if(!avg_pos) { cout << "Error: Out of memory!\n"; exit(1); }
	avg_neg = new double[dim];
	if(!avg_neg) { cout << "Error: Out of memory!\n"; exit(1); }
	sd_pos = new double[dim];
	if(!sd_pos) { cout << "Error: Out of memory!\n"; exit(1); }
	sd_neg = new double[dim];
	if(!sd_neg) { cout << "Error: Out of memory!\n"; exit(1); }
	
	//calc average
	for(i = 0; i < dim; ++i){
		num_neg = 0;
        num_pos = 0;
        avg_neg[i] = 0;
        avg_pos[i] = 0;
        for(j = 0; j < size; ++j){
            if(sample->points[j]->y == -1){
                avg_neg[i] += sample->points[j]->x[i];
                ++num_neg;
            }else{
                avg_pos[i] += sample->points[j]->x[i];
                ++num_pos;
            }
        }
        avg_neg[i] /= num_neg;
        avg_pos[i] /= num_pos;
	}
	
	//calc standart deviation
	for(i = 0; i < dim; ++i){
		sd_neg[i] = 0;
        sd_pos[i] = 0;
        for(j = 0; j < size; ++j){
            if(sample->points[j]->y == -1) sd_neg[i] += pow(sample->points[j]->x[i]-avg_neg[i], 2);
            else                          sd_pos[i] += pow(sample->points[j]->x[i]-avg_pos[i], 2);
        }
        sd_neg[i] = sqrt(sd_neg[i]/(num_neg-1));
        sd_pos[i] = sqrt(sd_pos[i]/(num_pos-1));
	}
	
	//alloc scores
	scores.resize(dim);
	
	//calc scores
	for(i = 0; i < dim; ++i){
		scores[i].score = fabs(avg_pos[i]-avg_neg[i])/(sd_pos[i]+sd_neg[i]);
        scores[i].fname = sample->fnames[i];
		if(verbose)
			cout << "Score: " << scores[i].score << ", Fname: " << scores[i].fname << endl;
	}
	if(verbose) cout << "----------------------------\n";
	if(verbose) cout << "Dim: " << dim << " -- " << endl;
	
	//training sample
	if(!train(sample, &w, margin, svs, 0)){
		delete [] w; w = NULL;
		if(verbose) cout << "Training failed!\n";
	}else{
		cout << "Training sucessful...\n";
		cout << "Margin = " << margin << ", Support Vectors = " << svs << endl;
		cout << "----------------------------\n";
	}
	
	delete [] avg_pos;
	delete [] avg_neg;
	delete [] sd_pos;
	delete [] sd_neg;
	
	sort(scores.begin(), scores.end(), golub_select_compare_score_greater);
	
	stmp_partial = Sample::copy(sample);
	
	//Alloc remove
	remove.resize(dim-number);

	for(i = 0;i < (dim-number); ++i){
		if(verbose) cout << "Score: " << scores[i].score << ", Fname: " << scores[i].fname << endl;
		remove[i] = scores[i].fname;
	
		stmp = Data::remove_features(sample, remove);
		
		if(verbose) cout << "Dim: " << dim - i -1 << " -- ";
		
		//training sample
		w = NULL;
		if(!train(stmp, &w, margin, svs, 0)){
			delete [] w; w = NULL;
			if(verbose) cout << "Training failed!\n";
			partial = 1;
			break;
		}else{
			cout << "Training successful...\n";
			cout << "Margin = " << margin << ", Support Vectors = " << svs << endl;
			cout << "----------------------------\n";
		}
		delete stmp_partial;
		stmp_partial = Sample::copy(stmp);
	}
	
	//save info
	remove.clear();
	scores.clear();
	if(w) delete [] w;

	if(partial){
		data.write(filename, stmp_partial, 0);
		delete stmp;
		return stmp_partial;
	}else{
		data.write(filename, stmp, 0);
		delete stmp_partial;
		return stmp;
	}
}

/*----------------------------------------------------------*
 * Returns 1 for a > b, -1 a < b, 0 if a = b                *
 *----------------------------------------------------------*/
bool golub_select_compare_score_greater(golub_select_score a, golub_select_score b){
	return (a.score < b.score);
}

