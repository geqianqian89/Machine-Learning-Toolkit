#include "../includes/Classifier.hpp"

using namespace std;

void Classifier::setSamples(Data *samples){
    this->samples = samples;
}

int Classifier::getSteps(){
    return steps;
}

int Classifier::getUpdates(){
    return ctot;
}
