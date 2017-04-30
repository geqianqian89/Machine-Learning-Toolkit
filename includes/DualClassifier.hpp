#ifndef DUALCLASSIFIER__HPP
#define DUALCLASSIFIER__HPP

#include "Kernel.hpp"
#include "Classifier.hpp"
#include <vector>

class DualClassifier : public Classifier {
    // Associations
    // Attributes
    private :
        /// Alphas vector.
        std::vector<double> alpha;
        /// Object for kernel computations.
        Kernel kernel;
};

#endif
