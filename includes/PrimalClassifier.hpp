#ifndef PRIMALCLASSIFIER__HPP
#define PRIMALCLASSIFIER__HPP

#include "Classifier.hpp"
#include <vector>

class PrimalClassifier : public Classifier {
    // Attributes
    private :
        /// Weights vector.
        std::vector<double> w;
};

#endif
