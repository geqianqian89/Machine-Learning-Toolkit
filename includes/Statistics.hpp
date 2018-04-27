#ifndef STATISTICS__HPP
#define STATISTICS__HPP

#include <vector>
#include <cmath>
#include <iostream>
#include "Point.hpp"
#include "Data.hpp"

template < typename T > class Data;

/**
 * \brief Class with methods for statistical computations.
 */

template  < typename T >
class Statistics {
    // Attributes
    friend class Data< T >;
private :
    /// Centroid of all the dataset.
    Point< T > centroid;
    /// Centroid of the positive points.
    Point< T > pos_centroid;
    /// Centroid of the negative points.
    Point< T > neg_centroid;
    /// Number of positive points.
    size_t n_pos = 0;
    /// Number of negative points.
    size_t n_neg = 0;
// Operations
public:
    explicit Statistics (){}
    /**
     * \brief Compute the mean (average) of a vector.
     * \param p (???) Point to compute the mean.
     * \return double
     */
    static double mean (std::vector< T > p);
    /**
     * \brief Computes the mean of a feature in the sample.
     * \param data (???) Sample where the feature is located.
     * \param index (???) Index of the feature to compute the mean.
     * \return double
     */
    static double getFeatureMean (Data< T > data, int index);
    /**
     * \brief Compute the variance of a vector.
     * \param p (???) Vector to compute the variance.
     * \return double
     */
    static double variance (std::vector< T > p);
    /**
     * \brief Compute the variance of a sample.
     * \param data (???) Sample to compute the variance.
     * \param index (???) Index of the feature to be ignored. (-1 dont ignore any feature)
     * \return double
     */
    static double variance (Data< T > data, int index);
    /**
     * \brief Compute the standard deviation of a vector.
     * \param p (???) Point to compute stdev.
     * \return double
     */
    static double stdev (std::vector< T > p);
    /**
     * \brief Computes the standard deviation of a feature.
     * \param data (???) Sample where the feature is located.
     * \param index (???) Index of teh feature to compute the standard deviation.
     * \return double
     */
    static double getFeatureStdev (Data< T > data, int index);
    /**
     * \brief Returns radius of the ball that circ. the data.
     * \param data  Dataset to compute the radius.
     * \param index Feature to be ignored (-1 uses all features).
     * \param q Lp-Norm to be used.
     * \return double
     */
    static double getRadius(Data< T > data, int index, double q);
    /**
     * \brief Returns distance of centers of the classes.
     * \param data  Dataset to compute the distance.
     * \param index Feature to be ignored (-1 uses all features).
     * \return double
     */
    static double getDistCenters(Data< T > data, int index);
    /**
     * \brief Returns distance of centers of the classes without given features.
     * \param data  Dataset to compute the distance.
     * \param feats Features to be excluded from the computation.
     * \param index Feature to be ignored (-1 uses all features).
     * \return double
     */
    static double getDistCentersWithoutFeats(Data< T > data, std::vector<int> feats, int index);
};

using namespace std;

template < typename T >
double Statistics< T >::mean(vector< T > p){
    int i, psize = p.size();
    double avg;

    for(i = 0, avg = 0; i < psize; i++){
        avg += p[i];
    }

    return (avg / psize);
}

template < typename T >
double Statistics< T >::getFeatureMean(Data< T > data, int index){
    int i, size = data.getSize();
    double sum = 0.0;
    vector<shared_ptr<Point< T > > > points = data.getPoints();

    for(i = 0; i < size; ++i){
        sum += points[i]->x[index];
    }
    sum /= size;

    return sum;
}

template < typename T >
double Statistics< T >::variance(vector< T > p){
    if(p.size() == 1) return 0.0;
    int i;
    double avg, sum, dim = p.size();

    avg = mean(p);

    for(sum = 0.0, i = 0; i < dim; ++i){
        sum += (p[i] - avg)*(p[i] - avg);
    }

    return (sum / dim);
}

template < typename T >
double Statistics< T >::variance(Data< T > data, int index){
    register int i, j;
    register double norm = 0.0;
    register double sum = 0.0;
    int dim = data.getDim(), size = data.getSize();
    vector<int> fnames = data.getFeaturesNames();
    vector<double> avg(dim);
    vector<shared_ptr<Point< T > > > points = data.getPoints();

    for(j = 0; j < dim; ++j){
        if(index < 0 || fnames[j] != index){
            avg[j] = 0.0;

            for(i = 0; i < size; ++i){
                avg[j] += points[i]->x[j];
            }
            avg[j] = avg[j] / size;
        }
    }

    for(i = 0; i < size; ++i){
        for(j = 0; j < dim; ++j){
            if(index < 0 || fnames[j] != index){
                norm += pow(avg[j] - points[i]->x[j], 2);
            }
            sum += norm;
        }
    }

    sum = sum/size;

    return sum;
}

template < typename T >
double Statistics< T >::stdev(vector< T > p){
    return sqrt(variance(p));
}

template < typename T >
double Statistics< T >::getFeatureStdev(Data< T > data, int index){
    int i, size = data.getSize();
    double avg, sd, vetsize = data.getDim();
    vector<shared_ptr<Point< T > > > points = data.getPoints();

    if(size == 1) return 0.0;

    avg = getFeatureMean(data, index);

    for(sd = 0.0, i = 0; i < vetsize; ++i){
        sd = (points[i]->x[index] - avg)*(points[i]->x[index] - avg);
    }

    return sqrt(sd/(vetsize - 1));
}

template < typename T >
double Statistics< T >::getRadius(Data< T > data, int index, double q){
    int i = 0, j = 0, dim = data.getDim(), size = data.getSize();
    double norm = 0.0;
    double max = 1.0;
    vector<int> fnames = data.getFeaturesNames();
    vector<double> avg(dim, 0.0);
    vector<shared_ptr<Point< T > > > points = data.getPoints();

    if(q == 2){
        for(j = 0; j < dim; ++j){
            if(index < 0 || fnames[j] != index){
                for(i = 0; i < size; ++i){
                    avg[j] += points[i]->x[j];
                }
                avg[j] = avg[j] / size;
            }
        }

        for(max = 0, i = 0; i < size; ++i){
            for(norm = 0, j = 0; j < dim; ++j){
                if(index < 0 || fnames[j] != index){
                    norm += pow(avg[j] - points[i]->x[j], 2);
                }

                norm = sqrt(norm);

                if(max < norm) max = norm;
            }
        }

    }else if(q == 1){
        for(max = 0, i = 0; i < size; ++i){
            for(j = 0; j < dim; ++j){
                if(index < 0 || fnames[j] != index)
                    if(max < fabs(points[i]->x[j]))
                        max = fabs(points[i]->x[j]);
            }
        }
    }

    return max;
}

template < typename T >
double Statistics< T >::getDistCenters(Data< T > data, int index){
    int i = 0, j = 0, dim = data.getDim(), size = data.getSize();
    double dist = 0.0;
    int size_pos = 0, size_neg = 0;
    vector<int> fnames = data.getFeaturesNames();
    vector<double> avg_pos(dim, 0.0), avg_neg(dim, 0.0);
    vector<shared_ptr<Point< T > > > points = data.getPoints();

    for(size_pos = 0, size_neg = 0, i = 0; i < size; ++i){
        if(points[i]->y == 1)	size_pos++;
        else 					size_neg++;
    }

    for(j = 0; j < dim; ++j){
        for(i = 0; i < size; ++i){
            if(points[i]->y == 1){
                avg_pos[j] += points[i]->x[j];
            }else
                avg_neg[j] += points[i]->x[j];
        }

        avg_pos[j] /= (double)size_pos;
        avg_neg[j] /= (double)size_neg;
    }

    for(dist = 0.0, j = 0; j < dim; ++j){
        if(index < 0 || fnames[j] != index)
            dist += pow(avg_pos[j] - avg_neg[j], 2);
    }

    return sqrt(dist);
}

template < typename T >
double Statistics< T >::getDistCentersWithoutFeats(Data< T > data, std::vector<int> feats, int index){
    int i = 0, j = 0, dim = data.getDim(), size = data.getSize();
    double dist = 0.0;
    int size_pos = 0, size_neg = 0, featsize = feats.size();
    vector<int> fnames = data.getFeaturesNames();
    vector<double> avg_pos(dim, 0.0), avg_neg(dim, 0.0);
    vector<shared_ptr<Point< T > > > points = data.getPoints();

    for(size_pos = 0, size_neg = 0, i = 0; i < size; ++i){
        if(points[i]->y == 1)	size_pos++;
        else					size_neg++;
    }

    for(j = 0; j < dim; ++j){
        for(i = 0; i < size; ++i){
            if(points[i]->y == 1)
                avg_pos[j] += points[i]->x[j];
            else
                avg_neg[j] += points[i]->x[j];
        }

        avg_pos[j] /= (double) size_pos;
        avg_neg[j] /= (double) size_neg;
    }

    for(dist = 0.0, j = 0; j < dim; ++j){
        for(i = 0; i < featsize; ++i){
            if(fnames[j] == feats[i])
                dist -= pow(avg_pos[j] - avg_neg[j], 2);
        }
    }

    return sqrt(fabs(dist));
}

#endif
