#ifndef KERNEL__HPP
#define KERNEL__HPP

#include <vector>

#include "../includes/Data.hpp"
#include "../includes/Utils.hpp"

/**
 * \brief Class for the kernel computations.
 */
class Kernel {
    // Attributes
    private :
        /// Kernel type and parameter.
        int type, param;
        /// Kernel matrix.
        dMatrix K;
    public :
        Kernel();
        Kernel(dMatrix K);
        void setType(int type);
        void setParam(int param);
        void setKernelMatrix(dMatrix K);
        dMatrix getKernelMatrix();
        void compute(Data samples);
        double function(Point one, Point two, int dim);
        /*!
         * \brief norm Computes norm in dual variables.
         * \param data Dataset to compute norm.
         * \return double
         */
        double norm(Data data);
};

#endif
