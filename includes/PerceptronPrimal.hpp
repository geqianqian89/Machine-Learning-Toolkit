#ifndef PERCEPTRONPRIMAL__HPP
#define PERCEPTRONPRIMAL__HPP

#include "PrimalClassifier.hpp"
#include "MLToolkit.hpp"

class PerceptronPrimal : public PrimalClassifier {
public:
    PerceptronPrimal(Data *samples = NULL, double q = 2, double rate = 0.5);
    bool train();
    double evaluate(Point p);
};

class PerceptronFixedMarginPrimal : public PrimalClassifier {
private:
    double gamma;

public:
    PerceptronFixedMarginPrimal(Data *samples = NULL, double gamma = 1.0, double q = 2, double rate = 0.5);
    bool train();
    double evaluate(Point p);
};
#endif
