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
    	/**
    	 * @brief Class constructor.
    	 */
        Kernel();
        /**
    	 * @brief Class constructor.
    	 * @param K Kernel matrix to be set in initialization.
    	 */
        Kernel(dMatrix K);
        /**
         * @brief setType Set the kernel type used in the kernel computations.
         * @param type Kernel type.
         */
        void setType(int type);
        /**
         * @brief setParam Set the kernel parameter used in the kernel computations.
         * @param param parameter to be set.
         */
        void setParam(int param);
        /**
         * @brief setKernelMatrix Set a pre computed kernel matrix.
         * @param K Kernel matrix to be set.
         */
        void setKernelMatrix(dMatrix K);
        /**
         * @brief getKernelMatrix Get the kernel matrix.
         * @return std::vector<std::vector<double> >
         */
        dMatrix getKernelMatrix();
        /**
         * @brief compute Compute the kernel matrix with the given type and parameter.
         * @param samples Data used to compute the kernel matrix.
         */
        void compute(Data samples);
        /**
         * @brief function Compute the kernel function between two points.
         * @param one first point.
         * @param two second point.
         * @param dim Dimension of the points.
         * @return double
         */
        double function(Point one, Point two, int dim);
        /**
         * @brief norm Computes norm in dual variables.
         * @param data Dataset to compute norm.
         * @return double
         */
        double norm(Data data);
};

#endif
