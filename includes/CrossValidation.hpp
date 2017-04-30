#ifndef CROSSVALIDATION__HPP
#define CROSSVALIDATION__HPP

#include "Classifier.hpp"
#include "Data.hpp"

class CrossValidation {
    // Attributes
    private :
        Classifier classifier;
        Data train_sample;
        Data test_sample;
        double initial_error;
        double limit_error;
        double actual_error;
    // Operations
    public :
        CrossValidation (Data sample, Classifier classifier);
        double kFold (int fold, int seed);
        void validation (int fold, int qtde);
        Data getTestSample ();
        Data getTrainSample ();
};

#endif
