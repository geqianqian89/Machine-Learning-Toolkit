#ifndef PRIMALCLASSIFIER__HPP
#define PRIMALCLASSIFIER__HPP

#include "Classifier.hpp"

class PrimalClassifier : public Classifier {
    // Attributes
    protected :
        /// Weights vector.
        std::vector<double> w;
        /// Norm used in the classification. (Euclidean Norm is the default)
        double q = 2, p = 2;
        /// Flexibility.
        double flexible = 0.0;
        /// Margin of the solution.
        double gamma = 0.0;
        /// Percentage of aproximation of the result.
        double alpha_aprox = 0.0;
    public:
        /**
         * @brief setqNorm Set the q norm used by the classifier. (Euclidean norm is the default)
         * @param q Norm that will be used by the classifier.
         */
        void setqNorm(double q);
        /**
         * @brief setpNorm Set the p norm used by the classifier. (Euclidean norm is the default)
         * @param p Norm that will be used by the classifier.
         */
        void setpNorm(double p);
        /**
         * @brief Set solution margin.
         * @param gamma Margin.
         */
        void setGamma(double gamma);
        /**
         * @brief Set flexibity of the classifier.
         * @param flexible flexibility.
         */
        void setFlexible(double flexible);
        /**
         * @brief Set the percentage of the aproximation.
         * @param alpha_aprox Aproximation.
         */
        void setAlphaAprox(double alpha_aprox);
        /**
         * @brief Get the margin of the solution.
         * @return double
         */
        double getGamma();
        /**
         * @brief getSolution Returns the solution of the primal classifier.
         * @return Solution
         */
        Solution getSolution();
};

#endif
