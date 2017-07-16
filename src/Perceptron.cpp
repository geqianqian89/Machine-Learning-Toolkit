#include <iostream>
#include <ctime>
#include <cmath>
#include "../includes/Perceptron.hpp"

using namespace std;

PerceptronPrimal::PerceptronPrimal(Data *samples, double q, double rate, Solution *initial_solution){
    this->samples = samples;
    this->q = q;
    this->rate = rate;
    if(initial_solution)
        this->solution = *initial_solution;
}

bool PerceptronPrimal::train(){
    int size = samples->getSize(), dim = samples->getDim();
    int i, j, e, idx;
    double norm, y, time = start_time + max_time, bias = 0;
    bool cond;
    vector<double> func(size, 0), w(dim, 0), x;
    vector<int> index = samples->getIndex();

    if(w.size() == 0) w.resize(dim);
	
	#ifdef __unix__
		cond = 100.0f*clock()/CLOCKS_PER_SEC-time <= 0; 
    #elif _WIN32
    	cond = 100.0f*clock()/CLOCKS_PER_SEC-time >= 0; 
    #endif
    
    while(cond){
    //while(1){
        for(e = 0, i = 0; i < size; ++i){
            idx = index[i];
            Point p = samples->getPoint(idx);
            x = p.x;
            y = p.y;

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
                ctot++; e++;
            }
            else if(steps > 0 && e > 1) break;
        }
        steps++;	//Number of iterations update

        //stop criterion
        if(e == 0) 		break;
        if(steps > MAX_IT) 	break;
        if(ctot > MAX_UP)	break;
        #ifdef __unix__
			cond = 100.0f*clock()/CLOCKS_PER_SEC-time <= 0; 
    	#elif _WIN32
    		cond = 100.0f*clock()/CLOCKS_PER_SEC-time >= 0; 
    	#endif
    }

    solution.bias = bias;
    solution.w = w;
    solution.norm = norm;

    return (e == 0);
}

double PerceptronPrimal::evaluate(Point p){
    int i, dim = solution.w.size();
    double bias = solution.bias, func;
    vector<double> w = solution.w;

    for(func = bias; i < dim; i++){
        func += w[i] * p.x[i];
    }
}

PerceptronFixedMarginPrimal::PerceptronFixedMarginPrimal(Data *samples, double gamma, double q, double rate, Solution *initial_solution){
    this->samples = samples;
    this->q = q;
    this->rate = rate;
    this->gamma = gamma;
    if(initial_solution)
        this->solution = *initial_solution;
}

bool PerceptronFixedMarginPrimal::train(){
    int i, j, k, e, s, r, dim = samples->getDim();
    int idx, sign = 1, size = samples->getSize(), n = dim, n_temp = 0, largn = 0;
    double norm = solution.norm, lambda = 1.0, y, time = start_time + max_time;
    double sumnorm = 0.0, bias = solution.bias, largw = 0.0, largw_temp = 0.0;
    bool cond;
    vector<double> func(size, 0.0), w = solution.w, x;
    vector<int> index = samples->getIndex();
    Point p;

    if(w.size() == 0) w.resize(dim);
    e = s = 0;
    
    #ifdef __unix__
		cond = 100.0f*clock()/CLOCKS_PER_SEC-time <= 0; 
    #elif _WIN32
    	cond = 100.0f*clock()/CLOCKS_PER_SEC-time >= 0; 
    #endif
    
    while(cond){
        for(e = 0, i = 0; i < size; ++i){
            idx = index[i];
            p = samples->getPoint(idx);
            x = p.x;
            y = p.y;

            //calculating function
            for(func[idx] = bias, j = 0; j < dim; ++j){
                func[idx] += w[j] * x[j];
            }

            //Checking if the point is a mistake
            if(y*func[idx] <= gamma*norm - samples->getPoint(idx).alpha*flexible){
                lambda = (norm) ? (1-rate*gamma/norm) : 1;
                for(r = 0; r < size; ++r){
                    Point b = samples->getPoint(r);
                    b.alpha *= lambda;
                    samples->setPoint(r, b);
                }

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
                p.alpha += rate;
                samples->setPoint(idx, p);

                k = (i > s) ? s++ : e;
                j = index[k];
                index[k] = idx;
                index[i] = j;
                ctot++; e++;
            }else if(steps > 0 && e > 1 && i > s) break;
        }
        ++steps; //Number of iterations update

        //stop criterion
        if(e == 0) 		break;
        if(steps > MAX_IT)	break;
        if(ctot > MAX_UP)	break;
        
        #ifdef __unix__
			cond = 100.0f*clock()/CLOCKS_PER_SEC-time <= 0; 
    	#elif _WIN32
    		cond = 100.0f*clock()/CLOCKS_PER_SEC-time >= 0; 
    	#endif
    }

    solution.norm = norm;
    solution.bias = bias;
    solution.w = w;

    return (e == 0);
}

double PerceptronFixedMarginPrimal::evaluate(Point p){
    int i, dim = solution.w.size();
    double bias = solution.bias, func;
    vector<double> w = solution.w;

    for(func = bias; i < dim; i++){
        func += w[i] * p.x[i];
    }
}

PerceptronDual::PerceptronDual(Data *samples, double rate, Kernel *K, Solution *initial_solution){
    this->samples = samples;
    if(initial_solution){
        this->solution = *initial_solution;
        this->alpha = (*initial_solution).alpha;
    }
    this->rate = rate;
    if(K)
        this->kernel = (*K);
}

bool PerceptronDual::train(){
    int y, e, i, j, idx, r, size = samples->getSize(), dim = samples->getDim();
    double norm = solution.norm, time = start_time+max_time;
    double bias = solution.bias;
    const double sqrate = rate * rate;
    const double tworate = 2 * rate;
    bool cond;
    vector<int> index = samples->getIndex();
    vector<double> func(size, 0.0), Kv;
    vector<Point> points = samples->getPoints();
    dMatrix K = kernel.getKernelMatrix();

    if(alpha.size() == 0){
        alpha.assign(size, 0.0);
    }

    e = 1;

    #ifdef __unix__
        cond = 100.0f*clock()/CLOCKS_PER_SEC-time <= 0;
    #elif _WIN32
        cond = 100.0f*clock()/CLOCKS_PER_SEC-time >= 0;
    #endif

    while(cond){
        for(e = 0, i = 0; i < size; ++i){
            idx = index[i];
            y = points[idx].y;

            Kv = K[idx];
            double f;

            //Calculating function
            for(f = bias, r = 0; r < size; ++r)
                f += alpha[r] * points[index[r]].y*Kv[index[r]];
            func[idx] = f;

            //Checking if the point is a mistake
            if(y * f <= 0.0){
                norm = sqrt(norm * norm + tworate*points[idx].y*func[idx] - bias + sqrate*Kv[idx]);
                alpha[i] += rate;
                bias += rate * y;
                ++ctot, ++e;
            }else if(steps > 0 && e > 1) break;
        }
        ++steps;

        //stop criterion
        if(e == 0)	   break;
        if(steps > MAX_IT) break;
        if(ctot > MAX_UP) break;

        #ifdef __unix__
            cond = 100.0f*clock()/CLOCKS_PER_SEC-time <= 0;
        #elif _WIN32
            cond = 100.0f*clock()/CLOCKS_PER_SEC-time >= 0;
        #endif
    }

    solution.bias = bias;
    solution.norm = norm;
    solution.alpha = alpha;
    solution.w.resize(dim);
    for(i = 0; i < dim; i++){
        for(j = 0; j < size; j++){
            solution.w[i] += alpha[j]*points[j].y*points[j].x[i];
        }
    }
    return (e == 0);
}

double PerceptronDual::evaluate(Point p){

}

PerceptronFixedMarginDual::PerceptronFixedMarginDual(Data *samples, double gamma, double rate, Kernel *K, Solution *initial_solution){
    this->samples = samples;
    //this->solution = *initial_solution;
    this->rate = rate;
    if(K)
        this->kernel = *K;
    this->gamma = gamma;
    if(initial_solution)
        this->alpha = (*initial_solution).alpha;
}

bool PerceptronFixedMarginDual::train(){
    int e, i, j, k, s, idx, r, size = samples->getSize(), dim = samples->getDim();
    double y, lambda, norm = solution.norm, time = start_time+max_time;
    double bias = solution.bias;
    const double sqrate  = rate*rate;
    const double tworate = 2*rate;
    bool cond;
    vector<int> index = samples->getIndex();
    vector<double> func(size, 0.0), Kv;
    vector<Point> points = samples->getPoints();
    dMatrix K = kernel.getKernelMatrix();

    if(alpha.size() == 0){
        alpha.assign(size, 0.0);
    }

    e = 1, s = 0;

    #ifdef __unix__
        cond = 100.0f*clock()/CLOCKS_PER_SEC-time <= 0;
    #elif _WIN32
        cond = 100.0f*clock()/CLOCKS_PER_SEC-time >= 0;
    #endif

    while(cond){
        for(e = 0, i = 0; i < size; ++i){
            idx = index[i];
            y = points[idx].y;

            //Checking if the point is a mistake
            if(y*func[idx] - gamma*norm <= 0){
                lambda = (gamma) ? (1-rate*gamma/norm) : 1;
                Kv     = K[idx];
                norm  *= lambda;

                for(r = 0; r < size; ++r){
                    alpha[r] *= lambda;
                    func[r]           = lambda * func[r] + rate*y*(Kv[r]+1) + bias*(1-lambda);
                }

                norm = sqrt(norm*norm + tworate*points[idx].y*lambda*(func[idx]-bias) + sqrate*Kv[idx]);
                alpha[idx] += rate;
                bias += rate * y;

                k = (i > s) ? ++s : e;
                j = index[k];
                index[k] = idx;
                index[i] = j;
                ++ctot; ++e;
            }else if(steps > 0 && e > 1 && i > s) break;
        }

        ++steps; //Number of iterations update

        //stop criterion
        if(e == 0)     break;
        if(steps > MAX_IT) break;
        if(ctot > MAX_UP) break;

        #ifdef __unix__
            cond = 100.0f*clock()/CLOCKS_PER_SEC-time <= 0;
        #elif _WIN32
            cond = 100.0f*clock()/CLOCKS_PER_SEC-time >= 0;
        #endif
    }

    solution.bias = bias;
    solution.norm = norm;
    solution.alpha = alpha;
    solution.w.resize(dim);
    for(i = 0; i < dim; i++){
        for(j = 0; j < size; j++){
            solution.w[i] += alpha[j]*points[j].y*points[j].x[i];
        }
    }

    return (e == 0);
}

double PerceptronFixedMarginDual::evaluate(Point p){

}
