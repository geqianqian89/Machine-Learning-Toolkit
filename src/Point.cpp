/**
  \brief Implementation of the Point class methods.
  \file Point.cpp
*/
#include <iostream>
#include <cmath>
#include "../includes/Point.hpp"
#include "../includes/Utils.hpp"

using namespace std;

double Point::dot(vector<double> p){
    int i, dim = p.size();
    double dot;

    if(dim != x.size()){
        cerr << "The points must have the same dimension!" << endl;
        return -1;
    }

    for(i = 0, dot = 0; i < dim; i++){
        dot += p[i] * x[i];
    }

    return dot;
}

double Point::norm(int p){
    int i, dim = x.size();
    double norm;

    if(p == NormType::NORM_LINF){
        return maxAbsElement(x);
    }

    for(i = 0, norm = 0; i < dim; i++){
        norm += pow(fabs(x[i]), p);
    }

    return pow(norm, 1.0/p);
}
