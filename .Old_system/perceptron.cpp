#include <iostream>
#include <cmath>
#include <ctime>
#include <cfloat>
#include <sys/time.h>
#include "data.h"
#include "perceptron.h"
#include "kernel.h"

#define MIN_INC  1.001
#define MAX_IT   1E9
#define MAX_UP   1E9
#define EPS      1E-9
#define MAX_PERC 1E-9

//Primal 2-norm
bool perceptron_primal_norm2(train_data *data, int& steps, int& ctot, int *index, double q, double start_time, double max_time, double rate){
	int c, e, i, j;
	int t, idx;
	int size = data->z->size, dim = data->z->dim;
	double norm = data->norm, y, time = start_time + max_time;
	double *func = data->func, *w = data->w;
	vector<double> x;
	vector<Point*> points = data->z->points;
	double bias = data->z->bias;
	
	t = steps, c = ctot, e = 0;
	
	while(100.0f*clock()/CLOCKS_PER_SEC-time <= 0){
		for(e = 0, i = 0; i < size; ++i){
			idx = index[i];
            x = points[idx]->x;
            y = points[idx]->y;

            //calculating function
            for(func[idx] = bias, j = 0; j < dim; ++j)
                func[idx] += w[j] * x[j];

            //Checking if the point is a mistake
            if(y*func[idx] <= 0.0){
                for(norm = 0.0, j = 0; j < dim; ++j){
                    w[j] += rate * y * x[j];
                    norm += w[j] * w[j];
                }
                norm = sqrt(norm);
                bias += rate * y;
                c++; e++;
            }
            else if(t > 0 && e > 1) break;
        }
		t++;	//Number of iterations update
		
		//stop criterion
		if(e == 0) 		break;
		if(t > MAX_IT) 	break;
		if(c > MAX_UP)	break;
	}
	data->norm = norm;
	data->z->bias = bias;
	steps = t;
	ctot = c;
	
	return (e == 0);
}

//Simple Dual
int perceptron_dual(datadual *data, int& steps, int& ctot, int *index, double start_time, double max_time, double rate){
	int y, t, c, e, i, idx, r, size = data->z->size;
	double norm = data->norm, time = start_time+max_time;
	double *func = data->func, *Kv = NULL;
	vector<Point*> points = data->z->points;
	double bias = data->z->bias;
	double **K = data->K;
	const double sqrate = rate * rate;
	const double tworate = 2 * rate;

	t = steps, c = ctot, e = 1;
	
	while(100.0f*clock()/CLOCKS_PER_SEC-time <= 0){
		for(e = 0, i = 0; i < size; ++i){
			idx = index[i];
			y = points[idx]->y;
			
			Kv = K[idx];
			double f;
			
			//Calculating function
			for(f = bias, r = 0; r < size; ++r)
				f += points[r]->alpha * points[index[r]]->y*Kv[index[r]];
			func[idx] = f;
			
			//Checking if the point is a mistake
			if(y * f <= 0.0){
				norm = sqrt(norm * norm + tworate*points[idx]->y*func[idx] - bias + sqrate*Kv[idx]);
				points[i]->alpha += rate;
				bias += rate * y;
				++c, ++e;
			}else if(t > 0 && e > 1) break;
		}
		++t;
		
		//stop criterion
		if(e == 0)	   break;
		if(t > MAX_IT) break;
		if(c > MAX_UP) break;
	}
	
	data->z->bias = bias;
	data->norm = norm;
	steps = t;
	ctot = c;

	return (e == 0);
}

//Fixed p-Margin Primal 
bool perceptron_fixed_margin_primal(train_data *data, double gamma, int* steps, int* ctot, int *index, double q, double start_time, double max_time, double rate){
	int c, e, i, k, s, j;
	int t, idx, r, sign = 1;
	int size = data->z->size, dim = data->z->dim;
	double norm = data->norm, lambda = 1.0, y, time = start_time+max_time;
	double *func = data->func, *w = data->w;
	vector<double> x;
	vector<Point*> points = data->z->points;	
	double sumnorm = 0.0; //Sum of the norms to future computation
	double bias = data->z->bias;
	double flexible = data->z->flexible;
	double largw = 0.0, largw_temp = 0.0;
	int n = dim, n_temp = 0, largn = 0;
	
	t = *steps, c = *ctot, e = 0, s = 0;
	
	while(100.0f*clock()/CLOCKS_PER_SEC-time <= 0){
		for(e = 0, i = 0; i < size; ++i){
			idx = index[i];
			x = points[idx]->x;
			y = points[idx]->y;
			
			//calculating function
			for(func[idx] = bias, j = 0; j < dim; ++j){
				func[idx] += w[j] * x[j];
			}
			
			//Checking if the point is a mistake
			if(y*func[idx] <= gamma*norm - points[idx]->alpha*flexible){
                lambda = (norm) ? (1-rate*gamma/norm) : 1;
                for(r = 0; r < size; ++r)
                    points[r]->alpha *= lambda;

                if(q == 1.0){ //Linf
                    for(sumnorm = 0.0, j = 0; j < dim; ++j){
                        sign = (w[j] < 0)? -1:1; 
                        lambda = (norm > 0 && w[j] != 0) ? gamma * sign: 0;
                        w[j] += rate * (y * x[j] - lambda);
                        sumnorm += fabs(w[j]);
                    }
                    norm = sumnorm;
                }
                else if(q == 2.0){ //L2
                    for(sumnorm = 0.0, j = 0; j < dim; ++j){
                        lambda = (norm > 0 && w[j] != 0) ? w[j] * gamma / norm : 0;
                        w[j] += rate * (y * x[j] - lambda);
                        sumnorm += w[j] * w[j];
                    }
                    norm = sqrt(sumnorm);
                }
                else if(q == -1.0){ //L1
                    largw_temp = fabs(w[0]);
                    n_temp = 1;
                    for(j = 0; j < dim; ++j){
                        if(largw == 0 || fabs(largw - fabs(w[j]))/largw < EPS){
                            sign = (w[j] < 0)? -1:1;
                            lambda = (norm > 0 && w[j] != 0) ? gamma * sign / n : 0;
                            w[j] += rate * (y * x[j] - lambda);
                        }
                        else
                            w[j] += rate * (y * x[j]);

                        if(j > 0){
                            if(fabs(largw_temp - fabs(w[j]))/largw_temp < EPS)
                                n_temp++;
                            else if(fabs(w[j]) > largw_temp){
                                largw_temp = fabs(w[j]);
                                n_temp = 1;
                            }
                        }
                    }
                    largw = largw_temp;
                    n = n_temp;
                    norm = largw;
                    if(n > largn) largn = n;
                }else{ //Other Lp formulations
                    for(sumnorm = 0, j = 0; j < dim; ++j){
                        lambda = (norm > 0 && w[j] != 0) ? w[j] * gamma * pow(fabs(w[j]), q-2.0) * pow(norm, 1.0-q) : 0;
                        w[j] += rate * (y * x[j] - lambda);
                        sumnorm += pow(fabs(w[j]), q);
                    }
                    norm = pow(sumnorm, 1.0/q);
                }
                bias += rate * y;
                points[idx]->alpha += rate;

                k = (i > s) ? s++ : e;
                j = index[k];
                index[k] = idx;
                index[i] = j;
                c++; e++;
            }else if(t > 0 && e > 1 && i > s) break;
		}
		t++; //Number of iterations update
		
		//stop criterion
		if(e == 0) 		break;
		if(t > MAX_IT)	break;
		if(c > MAX_UP)	break;
	}
	
	data->norm = norm;
	data->z->bias = bias;
	*steps = t;
	*ctot = c;
		
	return (e == 0);
}

/*Fixed Margin Dual*/
int perceptron_fixed_margin_dual(datadual *data, double gamma, int &steps, int &ctot, int *index, double start_time, double max_time, double rate){
	int t, c, e, i, j, k, s, idx, r, size = data->z->size;
	double y, lambda, norm = data->norm, time = start_time+max_time;
	double *func = data->func, *Kv = NULL;
	vector<Point*> points = data->z->points;
	double bias = data->z->bias;
	double **K = data->K;
	const double sqrate  = rate*rate;
	const double tworate = 2*rate;
	
	t = steps, c = ctot, e = 1, s = 0;
	
	while(100.0f*clock()/CLOCKS_PER_SEC-time <= 0){
		for(e = 0, i = 0; i < size; ++i){
        	idx = index[i];
        	y = points[idx]->y;

	        //Checking if the point is a mistake
	        if(y*func[idx] - gamma*norm <= 0){
	            lambda = (gamma) ? (1-rate*gamma/norm) : 1;
                Kv     = K[idx];
		        norm  *= lambda;

	            for(r = 0; r < size; ++r){
	                points[r]->alpha *= lambda;
                    func[r]           = lambda * func[r] + rate*y*(Kv[r]+1) + bias*(1-lambda);
	            }

	           	norm = sqrt(norm*norm + tworate*points[idx]->y*lambda*(func[idx]-bias) + sqrate*Kv[idx]);
                points[idx]->alpha += rate;
                bias += rate * y;

	            k = (i > s) ? ++s : e;
	            j = index[k];
	            index[k] = idx;
	            index[i] = j;
	            ++c; ++e;
			}else if(t > 0 && e > 1 && i > s) break;
		}
		
		++t; //Number of iterations update
		
		//stop criterion
		if(e == 0)     break;
    	if(t > MAX_IT) break;
    	if(c > MAX_UP) break;
	}
	
	data->z->bias = bias;
	data->norm = norm;
	steps = t;
	ctot = c;
	
	return (e == 0);
}

