#ifndef FEATURESELECTION__HPP
#define FEATURESELECTION__HPP

#include "Data.hpp"
#include "Classifier.hpp"
#include "Solution.hpp"

class FeatureSelection {
    // Attributes
    private :
        Data samples;
        Classifier classifier;
        Solution solution;
};

#endif
