#ifndef CLASSIFIER__HPP
#define CLASSIFIER__HPP

#include "Solution.hpp"
#include "Data.hpp"

#define MIN_INC  1.001
#define MAX_IT   1E9
#define MAX_UP   1E9

class Classifier {
    // Attributes
    protected :
        Data* samples;
        /// Support vectors points.
        std::vector<Point> svs;
        Solution solution;
        /// Learning rate
        double rate = 0.5;
        /// Initial time.
        double start_time = 0;
        /// Maximum time of training.
        double max_time = 200;
        /// Number of steps in the data.
        int steps = 0;
        /// Number of updates of the weights.
        int ctot = 0;
        /// Max precision.
        double EPS = 1E-9;
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
        virtual double evaluate (Point p) = 0;
        virtual void setSamples(Data *samples);
        int getSteps();
        int getUpdates();
};

#endif
