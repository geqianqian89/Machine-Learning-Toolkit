#include <iostream>
#include <ctime>
#include <cmath>
#include "../includes/PerceptronPrimal.hpp"

using namespace std;

PerceptronPrimal::PerceptronPrimal(Data *samples, double q, double rate){
    this->samples = samples;
    this->q = q;
    this->rate = rate;
}

void PerceptronPrimal::train(){
    int size = samples->getSize(), dim = samples->getDim();
    int i, j, e, idx;
    double norm, y, time = start_time + max_time, bias = solution.bias;
    vector<double> func(size, 0), w(dim, 0), x;
    vector<int> index = samples->getIndex();

    while(100.0f*clock()/CLOCKS_PER_SEC-time <= 0){
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
    }

    solution.bias = bias;
    solution.w = w;
    solution.norm = norm;

    return;
}

double PerceptronPrimal::evaluate(Point p){
    int i, dim = solution.w.size();
    double bias = solution.bias, func;
    vector<double> w = solution.w;

    for(func = bias; i < dim; i++){
        func += w[i] * p.x[i];
    }
}
