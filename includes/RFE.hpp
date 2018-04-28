//
// Created by mateus558 on 31/03/18.
//

#ifndef CLASSIFICATION_ALGORITHMS_SYSTEM_RFE_HPP
#define CLASSIFICATION_ALGORITHMS_SYSTEM_RFE_HPP

#include "FeatureSelection.hpp"

template < typename T >
class RFE : public FeatureSelection< T > {
public:

private:
    struct select_weight{
        double w;
        double val;
        double fname;
    };

    RFE(std::shared_ptr<Data< T > > samples = nullptr, Classifier< T >* classifier = nullptr, Validation::CrossValidation *cv = nullptr, int depth = 0, int skip = 0, int jump = 0, bool leave_one_out = false);
    std::unique_ptr<Data< T > > selectFeatures() override ;
    static int compare_weight_greater(const select_weight &a, const select_weight &b);
};

#endif //CLASSIFICATION_ALGORITHMS_SYSTEM_RFE_HPP
