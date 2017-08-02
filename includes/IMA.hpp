//
// Created by mateus558 on 01/08/17.
//

#ifndef CLASSIFICATION_ALGORITHMS_SYSTEM_IMA_HPP
#define CLASSIFICATION_ALGORITHMS_SYSTEM_IMA_HPP

#include <vector>
#include "PrimalClassifier.hpp"
#include "DualClassifier.hpp"
#include "MLToolkit.hpp"

class IMAp : public PrimalClassifier {
private:
    double margin;
    /// Support vector.
    std::vector<int> svs;

public:
    IMAp(Data *samples = NULL, double margin, Solution *initial_solution = NULL);
    bool train();
    double evaluate(Point p);
};


#endif //CLASSIFICATION_ALGORITHMS_SYSTEM_IMAP_HPP
