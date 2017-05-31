#ifndef SOLUTION__HPP
#define SOLUTION__HPP

#include <vector>

class Solution {
    // Attributes
    private :
        ///Weights vector
        std::vector<double> w;
        double bias;
        /// Features names of the resulting solution.
        std::vector<int> fnames;
        /// Margin generated from the classifier that generated the solution.
        double margin;
};

#endif
