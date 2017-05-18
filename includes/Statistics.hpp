#ifndef STATISTICS__HPP
#define STATISTICS__HPP

#include <vector>

#include "Point.hpp"

class Data;

/**
 * \brief Class with methods for statistical computations.
 */
class Statistics {
    // Attributes
    friend class Data;
private :
    /// Centroid of all the dataset.
    Point centroid;
    /// Centroid of the positive points.
    Point pos_centroid;
    /// Centroid of the negative points.
    Point neg_centroid;
    /// Number of positive points.
    int n_pos = 0;
    /// Number of negative points.
    int n_neg = 0;
// Operations
public:
    Statistics (){}
    /**
     * \brief Compute the mean (average) of a vector.
     * \param p (???) Point to compute the mean.
     * \return double
     */
    static double mean (std::vector<double> p);
    /**
     * \brief Computes the mean of a feature in the sample.
     * \param data (???) Sample where the feature is located.
     * \param index (???) Index of the feature to compute the mean.
     * \return double
     */
    static double getFeatureMean (Data data, int index);
    /**
     * \brief Compute the variance of a vector.
     * \param p (???) Vector to compute the variance.
     * \return double
     */
    static double variance (std::vector<double> p);
    /**
     * \brief Compute the variance of a sample.
     * \param data (???) Sample to compute the variance.
     * \param index (???) Index of the feature to be ignored. (-1 dont ignore any feature)
     * \return double
     */
    static double variance (Data data, int index);
    /**
     * \brief Compute the standard deviation of a vector.
     * \param p (???) Point to compute stdev.
     * \return double
     */
    static double stdev (std::vector<double> p);
    /**
     * \brief Computes the standard deviation of a feature.
     * \param data (???) Sample where the feature is located.
     * \param index (???) Index of teh feature to compute the standard deviation.
     * \return double
     */
    static double getFeatureStdev (Data data, int index);
    static double getRadius(Data data, int index, double q);
    static double getDistCenters(Data data, int index);
    static double getDistCentersWithoutFeats(Data data, std::vector<int> feats, int index);
};

#endif
