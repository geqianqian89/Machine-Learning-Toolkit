#ifndef KERNEL__HPP
#define KERNEL__HPP

#include <vector>

#include "../includes/Data.hpp"

/**
 * \brief Class for the kernel computations.
 */
class Kernel {
    // Attributes
    private :
        /// Kernel type.
        int type;
        /// Kernel matrix.
        std::vector<std::vector<double> > K;
    public :
        Kernel();
        /*!
         * \brief norm Computes norm in dual variables.
         * \param data Dataset to compute norm.
         * \return double
         */
        double norm(Data data);
};

#endif
