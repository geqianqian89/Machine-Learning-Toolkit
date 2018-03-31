#include "../includes/PrimalClassifier.hpp"

void PrimalClassifier::setqNorm(double q){
    this->q = q;
}

void PrimalClassifier::setpNorm(double p){
    this->p = p;
}

void PrimalClassifier::setFlexible(double flexible){
    this->flexible = flexible;
}

void PrimalClassifier::setAlphaAprox(double alpha_aprox) {
    this->alpha_aprox = alpha_aprox;
}
