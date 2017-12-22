#ifndef DUALCLASSIFIER__HPP
#define DUALCLASSIFIER__HPP

#include "Kernel.hpp"
#include "Classifier.hpp"
#include <vector>

class DualClassifier : public Classifier {
    // Associations
    // Attributes
    protected:
        /// Alphas vector.
        std::vector<double> alpha;
        /// Object for kernel computations.
        Kernel kernel;
    public:
        /**
         * @brief setKernel Set the kernel used by the dual classifier.
         * @param q Norm that will be used by the classifier.
         */
        void setKernel(Kernel K);
        /**
         * @brief getSolution Returns the solution of the primal classifier.
         * @return Solution
         */
        Solution getSolution();
        inline double getKernelParam(){ return kernel.getParam(); }
        inline double getKernelType(){ return kernel.getType(); }
};

#endif
