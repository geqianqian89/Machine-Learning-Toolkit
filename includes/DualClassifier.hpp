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
    inline std::string classifierType(){ return "Dual"; }

    /**
     * @brief setKernel Set the kernel used by the dual classifier.
     * @param q Norm that will be used by the classifier.
     */
    void setKernel(Kernel K);
    /**
     * @brief Get the parameter of the kernel.
     * @return double
     */
    inline double getKernelParam(){ return kernel.getParam(); }
    /**
     * @brief Get the type of the kernel.
     * @return double
     */
    inline double getKernelType(){ return kernel.getType(); }
};

#endif
