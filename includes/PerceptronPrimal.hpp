#ifndef PERCEPTRONPRIMAL__HPP
#define PERCEPTRONPRIMAL__HPP

#include "PrimalClassifier.hpp"
#include "MLToolkit.hpp"

class PerceptronPrimal : public PrimalClassifier {
public:
    PerceptronPrimal(Data *samples = NULL, double q = 2, double rate = 0.5);
    void train();
    double evaluate(Point p);
};

#endif
