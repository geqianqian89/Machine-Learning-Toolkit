/*****************************************************
 * fisher feature selection                          *
 *                                                   *
 * Saulo Moraes Villela <saulomv@gmail.com>          *
 * sep 08, 2011                                      *
 *****************************************************/
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include "data.h"
#include "utils.h"
#include "fisher.h"

Sample* fisher_select_features(string filename, Sample *sample, int(*train)(Sample*, double**, double&, int&, int), int number, int verbose){
	int i = 0, j = 0, num_pos = 0, num_neg = 0, svs = 0, partial = 0, dim = sample->dim, size = sample->size;
	double *w = NULL, margin = 0.0;
	vector<int> remove(dim - number);
	vector<double> avg_neg(dim), avg_pos(dim), sd_neg(dim), sd_pos(dim);
	vector<fisher_select_score> scores(dim);
	Sample *stmp = NULL, *stmp_partial = NULL; 
	Data data;

	//calc average
	for(i = 0; i < dim; ++i){
		num_neg = 0;
		num_pos = 0;
		avg_neg[i] = 0;
		avg_pos[i] = 0;
		
		for(j = 0; j < size; ++j){
			if(sample->points[j]->y == -1){
				avg_neg[i] += sample->points[j]->x[j];
				++num_neg;
			}else{
				avg_pos[i] += sample->points[j]->x[j];
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
			if(sample->points[j]->y == -1) sd_neg[i] += pow(sample->points[j]->x[j]-avg_neg[i], 2);
			else						   sd_pos[i] += pow(sample->points[j]->x[j]-avg_pos[i], 2);
		}
	}
	
	//calc scores
	for(i = 0; i < dim; ++i){
		scores[i].score = pow(avg_pos[i]-avg_neg[i], 2)/(sd_pos[i] + sd_neg[i]);
		scores[i].fname = sample->fnames[i];
		if(verbose)
			cout << "Score: " << scores[i].score << ", Fname: " << scores[i].fname << endl;
	}
	
	if(verbose) cout << "----------------------------\n";
	if(verbose) cout << "Dim: " << dim << " -- ";
	
	//training sample
	if(!train(sample, &w, margin, svs, 0)){
		delete w; w = NULL;
		if(verbose) cout << "Training failed!\n";
	}else{
		cout << "Training successful...\n";
		cout << "Margin = " << margin << ", Support Vectors = " << svs << endl;
		cout << "----------------------------\n";
	}
	
	sort(scores.begin(), scores.end(), fisher_select_compare_score_greater);
	
	stmp_partial = Sample::copy(sample);
	
	for(i = 0; i < (dim - number); ++i){
		if(verbose) cout << "Score: " << scores[i].score << ", Fname: " << scores[i].fname << endl;
		remove[i] = scores[i].fname;
		
		stmp = Data::remove_features(sample, remove);
		
		if(verbose)
			cout << "Dim: " << dim-i-1 << " -- ";
		
		//training sample
		w = NULL;
		if(!train(stmp, &w, margin, svs, 0)){
			delete w; w = NULL;
			if(verbose) cout << "Training failed!\n";
		}else{
			cout << "Training successful...\n";
			cout << "Margin = " << margin << ", Support Vectors = " << svs << endl;
			cout << "----------------------------\n";
		}
		delete stmp_partial;
		stmp_partial = Sample::copy(stmp);
	}
	
	//save info
	if(w) delete w;
	//delete sample;
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

bool fisher_select_compare_score_greater(fisher_select_score a, fisher_select_score b){
	return a.score < b.score; 
}
