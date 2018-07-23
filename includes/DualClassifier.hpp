/*! DualClassifier base class.
   \file DualClassifier.hpp
   \author Mateus Coutinho Marim
*/

#ifndef DUALCLASSIFIER__HPP
#define DUALCLASSIFIER__HPP

#include "Kernel.hpp"
#include "Classifier.hpp"
#include <vector>

template < typename T >
class DualClassifier : public Classifier< T > {
    // Associations
    // Attributes
protected:
    /// Alphas vector.
    std::vector<double> alpha;
    /// Object for kernel computations.
    Kernel *kernel;
public:
    inline std::string classifierType() override { return "Dual"; }

    /*********************************************
     *               Setters                     *
     *********************************************/

    /**
     * \brief setKernel Set the kernel used by the dual classifier.
     * \param q Norm that will be used by the classifier.
     */
    inline void setKernel(Kernel *K){ this->kernel = K; }
    /**
     * \brief Set the type of the kernel.
     * \param type The type of the selected kernel.
     */
    inline void setKernelType(int type){ kernel->setType(type); }
    /**
     * \brief Set the parameter of the kernel.
     * \param param The parameter of the selected kernel.
     */
    inline void setKernelParam(double param){ kernel->setType(param); }
    
    /**
     * \brief Get the parameter of the kernel.
     * \return double
     */
    inline double getKernelParam(){ return kernel->getParam(); }
    /**
     * \brief Get the type of the kernel.
     * \return double
     */
    inline double getKernelType(){ return kernel->getType(); }
    /**
     * \brief Get the vector of alphas.
     * \return std::vector<double>
     */
    inline std::vector<double> getAlphaVector() { return alpha; }
    /**
     * \brief Compute the weights of the dual classifier. 
     * \return std::vector<double>
     */
    std::vector<double> getDualWeight(){
        register int i = 0, j = 0, k = 0;
        size_t size = this->samples->getSize(), dim = this->samples->getDim();
        dMatrix *H, *Hk, matrixdif(size, std::vector<double>(size));
        std::vector<double> alphaaux(size);

        H = kernel->generateMatrixH(this->samples);

        this->solution.w.resize(dim);

        for(k = 0; k < dim; ++k)
        {
            Hk = kernel->generateMatrixHwithoutDim(this->samples, k);

            for(i = 0; i < size; ++i)
                for(j = 0; j < size; ++j)
                    matrixdif[i][j] = (*H)[i][j] - (*Hk)[i][j];

            for(i = 0; i < size; ++i)
                for(alphaaux[i] = 0, j = 0; j < size; ++j)
                    alphaaux[i] += this->samples->getPoint(j)->alpha * matrixdif[i][j];

            for(this->solution.w[k] = 0, i = 0; i < size; ++i)
                this->solution.w[k] += alphaaux[i] * this->samples->getPoint(i)->alpha;
        }

        return this->solution.w;
    }
    /**
     * \brief Compute the weights with inner product of the dual classifier. 
     * \return std::vector<double>
     */
    std::vector<double> getDualWeightProdInt(){
        register int i = 0, j = 0, k = 0;
        size_t size = this->samples->getSize(), dim = this->samples->getDim();
        std::vector<double> alphaaux(size);
        dMatrix H(size, std::vector<double>(size));

        this->solution.w.resize(dim);

        for(k = 0; k < dim; ++k)
        {
            for(i = 0; i < size; ++i)
                for(j = 0; j < size; ++j)
                    H[i][j] = (*this->samples)[i]->x[k] * (*this->samples)[j]->x[k]
                              * (*this->samples)[i]->y * (*this->samples)[j]->y;
            if(this->verbose >= 3) std::clog << "\n H matrix without dim generated.\n";
            for(i = 0; i < size; ++i)
                for(alphaaux[i] = 0, j = 0; j < size; ++j)
                    alphaaux[i] += this->samples->getPoint(j)->alpha * H[i][j];

            for(this->solution.w[k] = 0, i = 0; i < size; ++i)
                this->solution.w[k] += alphaaux[i] * this->samples->getPoint(i)->alpha;
        }

        return this->solution.w;
    }
};

#endif
