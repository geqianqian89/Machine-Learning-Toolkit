#ifndef VALIDATION__HPP
#define VALIDATION__HPP

#include "Classifier.hpp"
#include "Data.hpp"
#include "ValidationSolution.hpp"

/**
 * \brief Class of methods for the validation of ML algorithms.
 */
class Validation {
    // Attributes
    private :
        Classifier *classifier;
        Data *sample;
        Data train_sample, test_sample;
        double initial_error, limit_error, actual_error;
        double kernel_param;
        int kernel_type;
        int verbose = 1;
        /// Solution generated by the validation.
        ValidationSolution solution;
    // Operations
    public :
        Validation ();
        Validation (Data *sample, Classifier *classifier = NULL);
        /**
         * \brief Divide sample into train and test.
         * \param fold Number of folds.
         * \param seed  Seed to feed the pseudo random number generator.
         */
        void partTrainTest(int fold, unsigned int seed);
        double kFold (int fold, int seed);
        void validation (int fold, int qtde);
        Data getTestSample ();
        Data getTrainSample ();
};

#endif
