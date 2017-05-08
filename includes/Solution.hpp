#ifndef SOLUTION__HPP
#define SOLUTION__HPP

#include <vector>

class Solution {
    // Attributes
    private :
        /// Features names of the resulting solution.
        std::vector<int> fnames;
        /// Margin generated from the classifier that generated the solution.
        double margin;
};

#endif
