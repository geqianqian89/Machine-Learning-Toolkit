/*! PrimalClassifier base class
   \file PrimalClassifier.hpp
   \author Mateus Coutinho Marim
*/

#ifndef PRIMALCLASSIFIER__HPP
#define PRIMALCLASSIFIER__HPP

#include "Classifier.hpp"

template < typename T >
class PrimalClassifier : public Classifier< T > {
    // Attributes
protected :
    /// Weights vector.
    std::vector<double> w;
    /// Norm used in the classification. (Euclidean Norm is the default)
    double q = 2, p = 2;
    /// Flexibility.
    double flexible = 0.0;
    /// Percentage of aproximation of the result.
    double alpha_aprox = 0.0;
public:
    inline std::string classifierType() override { return "Primal"; }

    /*********************************************
     *               Setters                     *
     *********************************************/

    /**
     * \brief setqNorm Set the q norm used by the classifier. (Euclidean norm is the default)
     * \param q Norm that will be used by the classifier.
     */
    void setqNorm(double q) {this->q = q;}
    /**
     * \brief setpNorm Set the p norm used by the classifier. (Euclidean norm is the default)
     * \param p Norm that will be used by the classifier.
     */
    void setpNorm(double p) {this->p = p;}
    /**
     * \brief Set flexibity of the classifier.
     * \param flexible flexibility.
     */
    void setFlexible(double flexible) {this->flexible = flexible;}
    /**
     * \brief Set the percentage of the aproximation.
     * \param alpha_aprox Aproximation.
     */
    void setAlphaAprox(double alpha_aprox) {this->alpha_aprox = alpha_aprox;}
};

#endif
