#ifndef FEATURESELECTION__HPP
#define FEATURESELECTION__HPP

#include "Data.hpp"
#include "Classifier.hpp"
#include "Solution.hpp"
#include "Validation.hpp"

class FeatureSelection {
// Attributes
protected:
    std::shared_ptr<Data> samples;
    Classifier *classifier;
    Solution solution;
    Validation::CrossValidation *cv;

    int depth;
    int jump;
    int skip;
    bool leave_one_out;
    int verbose;

public:
    virtual std::unique_ptr<Data> selectFeatures() = 0;

    void setSamples(const std::shared_ptr<Data> &samples) {
        FeatureSelection::samples = samples;
    }

    void setClassifier(Classifier *classifier) {
        FeatureSelection::classifier = classifier;
    }

    void setSolution(const Solution &solution) {
        FeatureSelection::solution = solution;
    }

    void setDepth(int depth) {
        FeatureSelection::depth = depth;
    }

    void setJump(int jump) {
        FeatureSelection::jump = jump;
    }

    void setSkip(int skip) {
        FeatureSelection::skip = skip;
    }

    void setVerbose(int verbose){
        this->verbose = verbose;
    }
};

#endif
