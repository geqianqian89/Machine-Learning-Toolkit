#ifndef KERNEL__HPP
#define KERNEL__HPP

#include <vector>

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
};

#endif
