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

    std::vector<double> getDualWeight(){
        register int i = 0, j = 0, k = 0;
        size_t size = samples->getSize(), dim = samples->getDim();
        dMatrix H, Hk, matrixdif(size, std::vector<double>(size));
        std::vector<double> alphaaux(size);

        H = kernel->generateMatrixH(samples);

        solution.w.resize(dim);

        for(k = 0; k < dim; ++k)
        {
            Hk = kernel->generateMatrixHwithoutDim(samples, k);

            for(i = 0; i < size; ++i)
                for(j = 0; j < size; ++j)
                    matrixdif[i][j] = H[i][j] - Hk[i][j];

            for(i = 0; i < size; ++i)
                for(alphaaux[i] = 0, j = 0; j < size; ++j)
                    alphaaux[i] += samples->getPoint(j)->alpha * matrixdif[i][j];

            for(solution.w[k] = 0, i = 0; i < size; ++i)
                solution.w[k] += alphaaux[i] * samples->getPoint(i)->alpha;
        }

        return solution.w;
    }

    std::vector<double> getDualWeightProdInt(){
        register int i = 0, j = 0, k = 0;
        size_t size = samples->getSize(), dim = samples->getDim();
        std::vector<double> alphaaux(size);
        dMatrix H(size, std::vector<double>(size));

        solution.w.resize(dim);

        for(k = 0; k < dim; ++k)
        {
            for(i = 0; i < size; ++i)
                for(j = 0; j < size; ++j)
                    H[i][j] = samples->getPoint(i)->x[k] * samples->getPoint(j)->x[k] * samples->getPoint(i)->y * samples->getPoint(j)->y;

            for(i = 0; i < size; ++i)
                for(alphaaux[i] = 0, j = 0; j < size; ++j)
                    alphaaux[i] += samples->getPoint(j)->alpha * H[i][j];

            for(solution.w[k] = 0, i = 0; i < size; ++i)
                solution.w[k] += alphaaux[i] * samples->getPoint(i)->alpha;
        }

        return solution.w;
    }
};

#endif
