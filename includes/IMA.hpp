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
    IMAp(Data *samples = NULL, double margin = 0.0, Solution *initial_solution = NULL);

    bool train();
    double evaluate(Point p);
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
    IMApFixedMargin(Data *samples = NULL, double gamma = 0, Solution *initial_solution = NULL);

    bool train();
    inline int* getFlagNot1aDim() {return &flagNao1aDim; }
    inline unsigned long* gettMax(){ return &tMax; }
    double evaluate(Point p);
    void setNorm(double norm){ solution.norm = norm; }
};

#endif //CLASSIFICATION_ALGORITHMS_SYSTEM_IMAP_HPP
