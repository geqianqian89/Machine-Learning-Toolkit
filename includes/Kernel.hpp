#ifndef KERNEL__HPP
#define KERNEL__HPP

#include <vector>
#include <memory>

#include "../includes/Data.hpp"
#include "../includes/Utils.hpp"

/**
 * \brief Class for the kernel computations.
 */
class Kernel {
    // Attributes
private :
    /// Kernel type and parameter.
    int type;
    double param;
    /// Kernel matrix.
    dMatrix K;
public :
    /**
     * @brief Class constructor.
     */
    Kernel(int type = 0, double param = 0);
    dMatrix* getKernelMatrixPointer();
    /**
     * @brief Class constructor.
     * @param K Kernel matrix to be set in initialization.
     */
    Kernel(dMatrix kernel_matrix);
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
     * @brief getType Returns the kernel type used in the kernel computations.
     * @return int.
     */
    int getType();
    /**
     * @brief getParam Returns the kernel parameter used in the kernel computations.
     * @return double
     */
    double getParam();
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
    dMatrix generateMatrixH(std::shared_ptr<Data> samples);
    dMatrix generateMatrixHwithoutDim(std::shared_ptr<Data> samples, int dim);
    /**
     * @brief function Compute the kernel function between two points.
     * @param one first point.
     * @param two second point.
     * @param dim Dimension of the points.
     * @return double
     */
    double function(std::shared_ptr<Point> one, std::shared_ptr<Point> two, int dim);
    double functionWithoutDim(std::shared_ptr<Point> one, std::shared_ptr<Point> two, int j, int dim);

    /**
     * @brief norm Computes norm in dual variables.
     * @param data Dataset to compute norm.
     * @return double
     */
    double norm(Data data);
};

#endif
