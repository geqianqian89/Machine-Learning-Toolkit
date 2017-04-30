#ifndef CLASSIFIER__HPP
#define CLASSIFIER__HPP

#include "Data.hpp"
#include "Solution.hpp"
#include <vector>

class Classifier {
    // Attributes
    private :
        Data samples;
        /// Support vectors points.
        std::vector<Point> svs;
        Solution solution;
    // Operations
    public :
        /**
         * \brief Function that execute the training phase of a classification algorithm.
         * \return void
         */
        virtual void train () = 0;
        /**
         * \brief Returns the class of a feature point based on the trained classifier.
         * \param Point x (???) Features point to be evaluated.
         * \return int
         */
        virtual int evaluate ( Point x) = 0;
};

#endif
