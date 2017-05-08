#include <vector>
#include <cmath>
#include <iostream>

#include "../includes/Statistics.hpp"
#include "../includes/Data.hpp"

using namespace std;

double Statistics::mean(vector<double> p){
    int i, psize = p.size();
    double avg;

    for(i = 0, avg = 0; i < psize; i++){
        avg += p[i];
    }

    return (avg / psize);
}

double Statistics::variance(vector<double> p){
    if(p.size() == 1) return 0.0;
    int i;
    double avg, sum, dim = p.size();

    avg = mean(p);

    for(sum = 0.0, i = 0; i < dim; ++i){
        sum += (p[i] - avg)*(p[i] - avg);
    }

    return (sum / dim);
}

double Statistics::variance(Data data, int index){
    register int i, j;
    register double norm = 0.0;
    register double sum = 0.0;
    int dim = data.getDim(), size = data.getSize();
    vector<int> fnames = data.getFeaturesNames();
    vector<double> avg(dim);
    vector<Point> points = data.getPoints();

    for(j = 0; j < dim; ++j){
        if(index < 0 || fnames[j] != index){
            avg[j] = 0.0;

            for(i = 0; i < size; ++i){
                avg[j] += points[i].x[j];
            }
            avg[j] = avg[j] / size;
        }
    }

    for(i = 0; i < size; ++i){
        for(j = 0; j < dim; ++j){
            if(index < 0 || fnames[j] != index){
                norm += pow(avg[j] - points[i].x[j], 2);
            }
            sum += norm;
        }
    }

    sum = sum/size;

    return sum;
}

double Statistics::stdev(vector<double> p){
    return sqrt(variance(p));
}
