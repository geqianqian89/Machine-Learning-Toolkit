/*! ValidationSolution wrapper class
   \file ValidationSolution.hpp
   \author Mateus Coutinho Marim
*/

#ifndef VALIDATIONSOLUTION__HPP
#define VALIDATIONSOLUTION__HPP

#include "Solution.hpp"

/**
 * \brief Solution for the validation of a ML method.
 */
class ValidationSolution : public Solution {
    // Attributes
public:
    /// Accuracy of the validated model.
    double accuracy = 0.0;
    /// Precision of the validated model.
    double precision = 0.0;
    /// Recall of the validated model.
    double recall = 0.0;
    /// True negative rate.
    double tnrate = 0.0;
    /// True positive rate.
    double tprate = 0.0;
    /// Number of false positives generated by the classifier in the evaluation.
    size_t falsePositive = 0;
    /// Number of false negatives generated by the classifier in the evaluation.
    size_t falseNegative = 0;
    /// Number of true negatives generated by the classifier in the evaluation.
    size_t trueNegative = 0;
    /// Number of true positives generated by the classifier in the evaluation.
    size_t truePositive = 0;
};

#endif
