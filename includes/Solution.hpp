#ifndef SOLUTION__HPP
#define SOLUTION__HPP

#include <vector>

class Solution {
    // Attributes
    public :
        /// Weights vector
        std::vector<double> w;
        double bias = 0;
        /// Features names of the resulting solution.
        std::vector<int> fnames;
        /// Margin generated from the classifier that generated the solution.
        double margin = 0;
        double norm = 0;
};

#endif
