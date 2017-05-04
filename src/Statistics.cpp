#include <vector>

#include "../includes/Statistics.hpp"

using namespace std;

double Statistics::mean(vector<double> p){
    int i, psize = p.size();
    double avg;

    for(i = 0, avg = 0; i < psize; i++){
        avg += p[i];
    }

    return (avg / psize);
}

double Statistics::stdev(vector<double> p){
    if(p.size() == 1) return 0.0;
    int i;
    double avg, sd, psize = p.size();

    avg = mean(p);

    for(sd = 0.0, i = 0; i < psize; ++i){
        sd = (p[i] - avg)*(p[i] - avg);
    }

    return sqrt(sd/(p.size() - 1));
}
