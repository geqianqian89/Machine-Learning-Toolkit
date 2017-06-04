#ifndef PRIMALCLASSIFIER__HPP
#define PRIMALCLASSIFIER__HPP

#include "Classifier.hpp"

class PrimalClassifier : public Classifier {
    // Attributes
    protected :
        /// Weights vector.
        std::vector<double> w;
        /// Norm used in the classification. (Euclidean Norm is the default)
        double q = 2;
    public:
        void setNorm(double q);
        Solution getSolution();
};

#endif
