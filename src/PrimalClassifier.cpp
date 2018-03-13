#include "../includes/PrimalClassifier.hpp"

void PrimalClassifier::setqNorm(double q){
    this->q = q;
}

void PrimalClassifier::setpNorm(double p){
    this->p = p;
}

Solution PrimalClassifier::getSolution(){
    return solution;
}

void PrimalClassifier::setGamma(double gamma) {
    this->gamma = gamma;
}

double PrimalClassifier::getGamma() {
    return gamma;
}

void PrimalClassifier::setFlexible(double flexible){
    this->flexible = flexible;
}

void PrimalClassifier::setAlphaAprox(double alpha_aprox) {
    this->alpha_aprox = alpha_aprox;
}
