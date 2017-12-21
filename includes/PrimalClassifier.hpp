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
        /// Flexibilidade
        double flexible = 0.0;
        int verbose = 1;
        double gamma = 0.0;
        double alpha_aprox = 0.0;
    public:
        /**
         * @brief setNorm Set the norm used by the classifier. (Euclidean norm is the default)
         * @param q Norm that will be used by the classifier.
         */
        void setqNorm(double q);
        void setpNorm(double p);
        void setVerbose(int verbose);
        void setGamma(double gamma);
        void setFlexible(double flexible);
        void setAlphaAprox(double alpha_aprox);
        double getGamma();
        /**
         * @brief getSolution Returns the solution of the primal classifier.
         * @return Solution
         */
        Solution getSolution();
};

#endif
