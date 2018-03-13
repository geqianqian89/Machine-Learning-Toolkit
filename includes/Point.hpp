#ifndef POINT__HPP
#define POINT__HPP

#include <iostream>
#include <vector>

#include "../includes/Utils.hpp"

/**
 * \brief Class of a Point of doubles in a space of n dimensions.
 */
class Point {
    // Attributes
public :
    /// Features values.
    std::vector<double> x;
    /// Point classification.
    double y = 0, alpha = 0.0;
    /// Point identification.
    int id = 0;
    // Operations
    Point();
    Point(int dim, int val = 0);
    /**
     * \brief Computes the dot product with a vector.
     * \param p (???)
     * \return double
     */
    double dot (std::vector<double> p);
    /**
     * \brief Returns the p-norm of the point.
     * \param p (???) p of the norm (euclidean norm is the default).
     * \return double
     */
    double norm (int p = 2);

    /**********************************************
     *  Overloaded operators for the Point class. *
     **********************************************/

    friend std::ostream &operator<<( std::ostream &output, const Point &p );

    ~Point();
};
#endif
