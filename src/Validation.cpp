#include <random>
#include <functional>

#include "../includes/Validation.hpp"
#include "../includes/Data.hpp"
#include "../includes/Point.hpp"

using namespace std;

Validation::Validation(Data *sample, Classifier *classifier){
    this->sample = sample;
    this->classifier = classifier;
}

void Validation::partTrainTest(int fold, unsigned int seed){
    int i, j, npos, nneg, size = sample->getSize();
    Point p, aux;
    Data sample_pos, sample_neg;

    sample_pos = sample->copyZero();
    sample_neg = sample->copyZero();

    for(i = 0; i < size; i++){
        p = sample->getPoint(i);
        if(p.y == 1){
            sample_pos.insertPoint(*sample, i);
        }else{
            sample_neg.insertPoint(*sample, i);
        }
    }

    npos = sample_pos.getSize();
    nneg = sample_neg.getSize();

    auto dice_pos = bind(uniform_int_distribution<int>(0,npos-1), mt19937(seed));
    auto dice_neg = bind(uniform_int_distribution<int>(0,nneg-1), mt19937(seed));

    for(i = 0; i < npos; i++){
        j = dice_pos();
        aux = sample_pos.getPoint(i);
        sample_pos.setPoint(i, sample_pos.getPoint(j));
        sample_pos.setPoint(j, aux);
    }
    for(i = 0; i < nneg; i++){
        j = dice_neg();
        aux = sample_neg.getPoint(i);
        sample_neg.setPoint(i, sample_neg.getPoint(j));
        sample_neg.setPoint(j, aux);
    }

    train_sample = sample_pos.copyZero();
    test_sample = sample_pos.copyZero();

    for(j = 0; j < npos*(fold-1)/fold; j++)
        train_sample.insertPoint(sample_pos, j);
    for(; j < npos; j++)
        test_sample.insertPoint(sample_pos, j);

    for(j = 0; j < nneg/fold; j++)
        test_sample.insertPoint(sample_neg, j);
    for(; j < nneg; j++)
        train_sample.insertPoint(sample_neg, j);
}

double Validation::kFold(int fold, unsigned int seed){

}

void Validation::validation(int fold, int qtde){

}

Data Validation::getTestSample(){
    return test_sample;
}

Data Validation::getTrainSample(){
    return train_sample;
}
