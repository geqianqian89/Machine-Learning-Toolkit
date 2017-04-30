#ifndef VALIDATIONSOLUTION__HPP
#define VALIDATIONSOLUTION__HPP

#include "Solution.hpp"

/**
 * \brief Solution for the validation of a ML method.
 */
class ValidationSolution : public Solution {
    // Attributes
    private :
        /// Accuracy of the validated model.
        double accuracy = 0;
};

#endif
