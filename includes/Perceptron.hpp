#ifndef PERCEPTRONPRIMAL__HPP
#define PERCEPTRONPRIMAL__HPP

/*! Perceptron algorithm implementations
   \file Perceptron.hpp
   \author Mateus Coutinho Marim
*/

#include "PrimalClassifier.hpp"
#include "DualClassifier.hpp"
#include "MLToolkit.hpp"

/**
 * \brief Wrapper for the implementation of the Perceptron primal algorithm.
 */

class PerceptronPrimal : public PrimalClassifier {
public:
    explicit PerceptronPrimal(Data *samples = nullptr, double q = 2, double rate = 0.5, Solution *initial_solution = nullptr);
    bool train() override;
    double evaluate(Point p) override;
};

/**
 * \brief Wrapper for the implementation of the Perceptron primal with fixed margin algorithm.
 */
class PerceptronFixedMarginPrimal : public PrimalClassifier {
public:
    explicit PerceptronFixedMarginPrimal(Data *samples = nullptr, double gamma = 1.0, double q = 2, double rate = 0.5, Solution *initial_solution = nullptr);
    bool train() override;
    double evaluate(Point p) override;
};

/**
 * \brief Wrapper for the implementation of the Perceptron dual algorithm.
 */
class PerceptronDual : public DualClassifier {
public:
    explicit PerceptronDual(Data *samples = nullptr, double rate = 0.5, Kernel *K = nullptr, Solution *initial_solution = nullptr);
    bool train() override;
    double evaluate(Point p) override;
};

/**
 * \brief Wrapper for the implementation of the Perceptron dual with fixed margin algorithm.
 */
class PerceptronFixedMarginDual : public DualClassifier {
public:
    explicit PerceptronFixedMarginDual(Data *samples = nullptr, double gamma = 1.0, double rate = 0.5, Kernel *K = nullptr, Solution *initial_solution = nullptr);
    bool train() override;
    double evaluate(Point p) override;
};

#endif
