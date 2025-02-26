//
// Created by Mateus Coutinho Marim on 6/26/2018.
//

#ifndef UFJF_MLTK_FEATURESELECTION_H
#define UFJF_MLTK_FEATURESELECTION_H

#include "Data.hpp"
#include "Classifier.hpp"
#include "Validation.hpp"

#include <memory>

template < typename T >
class FeatureSelection {
/// Attributes
protected:
    /// Samples where the features will be selected
    std::shared_ptr<Data< T > > samples;
    /// Classifier used by the method
    Classifier< T > *classifier;
    /// Structure to hold the cross-validation result
    typename Validation< T >::CrossValidation *cv;
    /// Solution of the feature selection
    Solution solution;

    int depth = 0;
    int final_dim = 0;
    int jump = 0;
    int skip = 0;
    bool leave_one_out = false;
    int verbose = 0;

public:
    virtual std::shared_ptr<Data< T > > selectFeatures() = 0;

    void setSamples(const std::shared_ptr<Data< T > > &samples) {
        this->samples = samples;
    }

    void setClassifier(Classifier< T > *classifier) {
        this->classifier = classifier;
    }

    void setFinalDimension(int final_dim){ this->final_dim = final_dim; }

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

    void setCrossValidation(typename Validation< T >::CrossValidation *cv){
        this->cv = cv;
    }
};

#endif //UFJF_MLTK_FEATURESELECTION_H
