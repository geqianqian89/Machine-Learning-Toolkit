/*#ifndef FEATURESELECTION__HPP
#define FEATURESELECTION__HPP

#include "Data.hpp"
#include "Classifier.hpp"
#include "Solution.hpp"
#include "Validation.hpp"

template< typename T >
class FeatureSelection {
// Attributes
protected:
    std::shared_ptr<Data< T > > samples;
    Classifier< T > *classifier;
    Solution solution;
    Validation< T >::CrossValidation *cv;

    int depth;
    int jump;
    int skip;
    bool leave_one_out;
    int verbose;

public:
    virtual std::unique_ptr<Data< T > > selectFeatures() = 0;

    void setSamples(const std::shared_ptr<Data< T > > &samples) {
        this->samples = samples;
    }

    void setClassifier(Classifier< T > *classifier) {
        this->classifier = classifier;
    }

    void setSolution(const Solution &solution) {
        this->solution = solution;
    }

    void setDepth(int depth) {
        this->depth = depth;
    }

    void setJump(int jump) {
        this->jump = jump;
    }

    void setSkip(int skip) {
        this->skip = skip;
    }

    void setVerbose(int verbose){
        this->verbose = verbose;
    }
};

#endif
*/