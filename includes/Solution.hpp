#ifndef SOLUTION__HPP
#define SOLUTION__HPP

#include <vector>
#include "../includes/Kernel.hpp"

class Solution {
    // Attributes
    public :
        /// Weights vector
        std::vector<double> w;
        ///Kernel for Dual methods
        Kernel K;
        /// Alpha Vector for Dual methods
        std::vector<double> alpha;
        /// Bias of the solution.
        double bias = 0;
        /// Features names of the resulting solution.
        std::vector<int> fnames;
        /// Margin generated from the classifier that generated the solution.
        double margin = 0;
        /// Norm of the solution.
        double norm = 0;
};

#endif
