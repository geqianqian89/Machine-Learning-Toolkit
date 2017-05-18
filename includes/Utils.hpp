#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

/*! Utils functions
   \file Utils.hpp
   \author Mateus Coutinho Marim
*/

#include <string>
#include <vector>
#define INF 1E8

enum NormType {NORM_LINF = 0, NORM_L1 = 1, NORM_L2 = 2};
typedef std::vector<std::vector<double> > dMatrix;

/*!
 * \brief Verify if the string is a number.
 * \param str   String to be tested.
 * \return  bool
 */
bool is_number(std::string str);

/*!
 * \brief Converts the string to an integer.
 * \param str   String to be converted.
 * \return The integer resulted from the conversion.
 */
int stoin(std::string str);

/*!
 * \brief Converts the string to a double.
 * \param str   The string to be converted.
 * \return  The double resulted from the conversion.
 */
double stodn(std::string str);

/*!
 * \brief Returns the max absolute element.
 * \param x The vector used to obtain the max element.
 * \return  The max absolute element found.
 */
double maxAbsElement(std::vector<double> x);

#endif
