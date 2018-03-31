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
    Kernel *kernel;
public:
    inline std::string classifierType(){ return "Dual"; }

    /**
     * @brief setKernel Set the kernel used by the dual classifier.
     * @param q Norm that will be used by the classifier.
     */
    inline void setKernel(Kernel *K){ this->kernel = K; }
    /**
     * @brief Get the parameter of the kernel.
     * @return double
     */
    inline double getKernelParam(){ return kernel->getParam(); }
    /**
     * @brief Get the type of the kernel.
     * @return double
     */
    inline double getKernelType(){ return kernel->getType(); }

    inline void setKernelType(int type){ kernel->setType(type); }

    inline void setKernelParam(double param){ kernel->setType(param); }

    void getDualWeight(){
        size_t i, j;
        size_t dim = samples->getDim(), size = samples->getSize();

        solution.w.resize(dim);

        for(j = 0; j < dim; ++j)
            for(solution.w[j] = 0, i = 0; i < size; ++i)
                solution.w[j] += samples->getPoint(i)->alpha * samples->getPoint(i)->y * samples->getPoint(i)->x[j];
    }

    void getDualWeightProdInt(Data samples);
};

#endif
