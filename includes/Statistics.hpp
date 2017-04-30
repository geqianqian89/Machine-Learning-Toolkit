#ifndef STATISTICS__HPP
#define STATISTICS__HPP

#include "Point.hpp"
#include <vector>

/**
 * \brief Class with methods for statistical computations.
 */
class Statistics {
    // Attributes
    public :
        /// Centroid of the positive points.
        Point pos_centroid;
        /// Centroid of the negative points.
        Point neg_centroid;
        /// Number of positive points.
        int n_pos = 0;
        /// Number of negative points.
        int n_neg = 0;
    // Operations
        Statistics (){}
        /**
         * \brief Compute the standard deviation of a vector.
         * \param p (???) Point to compute stdev.
         * \return double
         */
        static double stdev (std::vector<double> p);
        /**
         * \brief Compute the mean (average) of a vector.
         * \param p (???) Point to compute the mean.
         * \return double
         */
        static double mean (std::vector<double> p);
};

#endif
