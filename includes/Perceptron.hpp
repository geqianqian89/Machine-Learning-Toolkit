#ifndef PERCEPTRONPRIMAL__HPP
#define PERCEPTRONPRIMAL__HPP

#include "PrimalClassifier.hpp"
#include "DualClassifier.hpp"
#include "MLToolkit.hpp"

/**
 * \brief Wrapper for the implementation of the Perceptron primal algorithm.
 */

class PerceptronPrimal : public PrimalClassifier {
public:
    PerceptronPrimal(Data *samples = NULL, double q = 2, double rate = 0.5, Solution *initial_solution = NULL);
    bool train();
    double evaluate(Point p);
};

/**
 * \brief Wrapper for the implementation of the Perceptron primal with fixed margin algorithm.
 */
class PerceptronFixedMarginPrimal : public PrimalClassifier {
private:
    double gamma;

public:
    PerceptronFixedMarginPrimal(Data *samples = NULL, double gamma = 1.0, double q = 2, double rate = 0.5, Solution *initial_solution = NULL);
    bool train();
    double evaluate(Point p);
};

/**
 * \brief Wrapper for the implementation of the Perceptron dual algorithm.
 */
class PerceptronDual : public DualClassifier {
public:
    PerceptronDual(Data *samples = NULL, double rate = 0.5, Kernel *K = NULL, Solution *initial_solution = NULL);
    bool train();
    double evaluate(Point p);
};

/**
 * \brief Wrapper for the implementation of the Perceptron dual with fixed margin algorithm.
 */
class PerceptronFixedMarginDual : public DualClassifier {
private:
    double gamma;
public:
    PerceptronFixedMarginDual(Data *samples = NULL, double gamma = 1.0, double rate = 0.5, Kernel *K = NULL, Solution *initial_solution = NULL);
    bool train();
    double evaluate(Point p);
};

#endif
