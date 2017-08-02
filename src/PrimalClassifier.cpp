#include "../includes/PrimalClassifier.hpp"

void PrimalClassifier::setNorm(double q){
    this->q = q;
}

Solution PrimalClassifier::getSolution(){
    return solution;
}

void PrimalClassifier::setVerbose(int verbose) {
    this->verbose = verbose;
}
