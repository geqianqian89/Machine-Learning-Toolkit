//
// Created by mateus558 on 01/08/17.
//

#ifndef CLASSIFICATION_ALGORITHMS_SYSTEM_IMA_HPP
#define CLASSIFICATION_ALGORITHMS_SYSTEM_IMA_HPP

#include <vector>
#include "PrimalClassifier.hpp"
#include "DualClassifier.hpp"
#include "MLToolkit.hpp"

/**
 * \brief Wrapper for the implementation of the Incremental Margin Algorithm primal.
 */
class IMAp : public PrimalClassifier {
private:
    double margin;
    /// Support vector.
    std::vector<int> svs;

public:
    explicit IMAp(Data *samples = nullptr, double margin = 0.0, Solution *initial_solution = nullptr);

    bool train() override;
    double evaluate(Point p) override;
};

/**
 * \brief Wrapper for the implementation of the Incremental Margin Algorithm primal with fixed margin.
 */
class IMApFixedMargin : public PrimalClassifier {
private:
    int n, maiorn = 0, flagNao1aDim;
    double margin = 0.0, maiorw = 0.0;
    unsigned long tMax;

    /// Support vector.
    std::vector<int> svs;

public:
    explicit IMApFixedMargin(Data *samples = nullptr, double gamma = 0, Solution *initial_solution = nullptr);

    bool train() override;
    inline int* getFlagNot1aDim() {return &flagNao1aDim; }
    inline unsigned long* gettMax(){ return &tMax; }
    double evaluate(Point p) override;
    void setNorm(double norm){ solution.norm = norm; }
};

class IMADual : public DualClassifier {
private:
    double margin = 0;
public:
    explicit IMADual(Data *samples = nullptr, Kernel *k = nullptr, double rate = 1, Solution *initial_solution = nullptr);
    bool train() override;
    double evaluate(Point p) override;
};

#endif //CLASSIFICATION_ALGORITHMS_SYSTEM_IMAP_HPP
