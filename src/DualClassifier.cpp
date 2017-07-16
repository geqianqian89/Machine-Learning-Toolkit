#include "../includes/DualClassifier.hpp"

void DualClassifier::setKernel(Kernel K){
    this->kernel = K;
}

Solution DualClassifier::getSolution(){
    return solution;
}
