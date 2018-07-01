/*! Kernel manipulation class
   \file Kernel.hpp
   \author Mateus Coutinho Marim
*/

#ifndef KERNEL__HPP
#define KERNEL__HPP

#include <vector>
#include <memory>
#include <cmath>
#include <utility>

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
    /// Kernel parameter.
    double param;
    /// Kernel matrix.
    dMatrix K;
    /// H matrix.
    dMatrix H;
    /// H matrix without a dimension.
    dMatrix HwithoutDim;
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
    template < typename T >
    void compute(std::shared_ptr<Data< T > > samples);
    /**
     * @brief compute Compute the H matrix with the computed kernel matrix and given samples.
     * @param samples Data used to compute the kernel matrix.
     * @return dMatrix*
     */
    template < typename T >
    dMatrix* generateMatrixH(std::shared_ptr<Data< T > > samples);
    /**
     * @brief compute Compute the H matrix without a dimension, with the computed kernel matrix and given samples.
     * @param samples Data used to compute the kernel matrix.
     * @param dim dimension to be ignored.
     * @return dMatrix*
     */
    template < typename T >
    dMatrix* generateMatrixHwithoutDim(std::shared_ptr<Data< T > > samples, int dim);
    /**
     * @brief function Compute the kernel function between two points.
     * @param one first point.
     * @param two second point.
     * @param dim Dimension of the points.
     * @return double
     */
    template < typename T >
    double function(shared_ptr<Point< T > > one, shared_ptr<Point< T > > two, int dim);
    /**
     * @brief function Compute the kernel function between two points without a dimension.
     * @param one first point.
     * @param two second point.
     * @param j Dimension to be ignored.
     * @param dim Dimension of the points.
     * @return double
     */
    template < typename T >
    double functionWithoutDim(std::shared_ptr<Point< T > > one, std::shared_ptr<Point< T > > two, int j, int dim);

    /**
     * @brief norm Computes norm in dual variables.
     * @param data Dataset to compute norm.
     * @return double
     */
    template < typename T >
    double norm(Data< T > data);
};

template < typename T >
void Kernel::compute(const std::shared_ptr<Data< T > > samples){
    size_t i, j, size = samples->getSize(), dim = samples->getDim();

    K.assign(size, vector<double>(size, 0.0));

    //Calculating Matrix
    for(i = 0; i < size; ++i){
        for(j = i; j < size; ++j){
            K[i][j] = function((*samples)[i], (*samples)[j], dim);
            K[j][i] = K[i][j];
        }
    }
}

template < typename T >
dMatrix* Kernel::generateMatrixH(const std::shared_ptr<Data< T > > samples) {
    register int i = 0, j = 0;
    size_t size = samples->getSize(), dim = samples->getDim();

    H.resize(size, vector<double>(size));

    /* Calculating Matrix */
    for(i = 0; i < size; ++i) {
        for (j = i; j < size; ++j) {
            H[i][j] = function(samples->getPoint(i), samples->getPoint(j), dim) * samples->getPoint(i)->y *
                      samples->getPoint(j)->y;
            H[j][i] = H[i][j];
        }
    }
    clog << "\nH matrix generated.\n";
    return &H;
}

template < typename T >
dMatrix* Kernel::generateMatrixHwithoutDim(const std::shared_ptr<Data< T > > samples, int dim) {
    register int i = 0, j = 0;
    size_t size = samples->getSize();

    HwithoutDim.resize(size, vector<double>(size));

    /* Calculating Matrix */
    for(i = 0; i < size; ++i) {
        for (j = i; j < size; ++j) {
            HwithoutDim[i][j] = functionWithoutDim((*samples)[i], (*samples)[j], dim, samples->getDim()) *
                                samples->getPoint(i)->y * samples->getPoint(j)->y;
            HwithoutDim[j][i] = HwithoutDim[i][j];
        }
    }
   // clog << "\nH matrix without dim generated.\n";
    return &HwithoutDim;
}

template < typename T >
double Kernel::function(shared_ptr<Point< T > > one, shared_ptr<Point< T > > two, int dim){
    int i = 0;
    double t, sum = 0.0;
    vector< T > a = one->x, b = two->x;

    // a.erase(a.end());
    //b.erase(b.end());

    switch(type)
    {
        case 0: //Produto Interno
            for(i = 0; i < dim; ++i)
                sum += a[i] * b[i];
            break;
        case 1: //Polinomial
            for(i = 0; i < dim; ++i)
                sum += a[i] * b[i];
            //    sum = (param > 1) ? pow(sum+1, param) : sum;
            sum = (param > 1) ? pow(sum, param) : sum;
            break;

        case 2: //Gaussiano
            for(i = 0; i < dim; ++i)
            { t = a[i] - b[i]; sum += t * t; }
            sum = exp(-1 * sum * param);
            break;
    }

    /*The '+1' here accounts for the bias term "b" in SVM formulation since
      <w,x> = \sum_i \alpha_i y_i k(x_i,x) + b and b=\sum_i \alpha_i y_i*/

    return sum;// + 1.0f;
}

template < typename T >
double Kernel::functionWithoutDim(std::shared_ptr<Point< T > > one, std::shared_ptr<Point< T > > two, int j, int dim) {
    int i = 0;
    double t, sum = 0.0;

    switch(type)
    {
        case 0: //Produto Interno
            for(i = 0; i < dim; ++i)
                if(i != j)
                    sum += one->x[i] * two->x[i];
            break;

        case 1: //Polinomial
            for(i = 0; i < dim; ++i)
                if(i != j)
                    sum += one->x[i] * two->x[i];
            sum = (param > 1) ? pow(sum+1, param) : sum;
            break;

        case 2: //Gaussiano
            for(i = 0; i < dim; ++i)
                if(i != j)
                { t = one->x[i] - two->x[i]; sum += t * t; }
            sum = exp(-1 * sum * param);
            break;
    }
    /*The '+1' here accounts for the bias term "b" in SVM formulation since
      <w,x> = \sum_i \alpha_i y_i k(x_i,x) + b and b=\sum_i \alpha_i y_i*/
    return sum;// + 1.0f;
}

template < typename T >
double Kernel::norm(Data< T > data){
    size_t i, j, size = data.getSize();
    double sum, sum1;
    vector<shared_ptr<Point< T > > > points = data.getPoints();

    sum = sum1 = 0;

    for(i = 0; i < size; ++i){
        for(j = 0; j < size; j++){
            sum1 += points[j]->alpha * points[j]->y * K[i][j];
            sum += points[i]->y * points[i]->alpha * sum1;
        }
    }

    return sqrt(sum);
}


#endif
