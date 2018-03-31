#include "../includes/Classifier.hpp"

using namespace std;

void Classifier::setSamples(std::shared_ptr<Data> samples){
    this->samples = samples;
}

void Classifier::setStartTime(double start_time){
    this->start_time = start_time;
}

void Classifier::setMaxTime(double max_time){
    this->max_time = max_time;
}

void Classifier::setEPS(double EPS){
    this->EPS = EPS;
}

void Classifier::setMaxIterations(int MAX_IT){
    this->MAX_IT = MAX_IT;
}

void Classifier::setVerbose(int verbose) {
    this->verbose = verbose;
}

void Classifier::setMaxUpdates(int MAX_UP){
    this->MAX_UP = MAX_UP;
}

void Classifier::setCtot(int ctot) {
    this->ctot = ctot;
}

void Classifier::setSolution(Solution solution){
    this->solution = solution;
}

Solution Classifier::getSolution(){
    return solution;
}

void Classifier::setLearningRate(double rate){
    this->rate = rate;
}
