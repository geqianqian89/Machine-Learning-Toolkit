#ifndef VISUALISATION__HPP
#define VISUALISATION__HPP

#include "Data.hpp"
#include "gnuplot_i.hpp"

#include <string>

/**
 * \brief Class for visualize data using gnuplot.
 */
class Visualisation {
    // Attributes
    private :
        /// Sample to be visualized.
        Data *samples;
        /// Interface to gnuplot.
        Gnuplot g;
    // Operations
    public :
        Visualisation ();
        Visualisation (Data *sample);
        /**
         * \brief Set sample to be visualized.
         * \param sample (???) Data to set for visualization.
         * \return void
         */
        void setSample (Data sample);
        /**
         * \brief Set plot title.
         * \param title (???) Plot title.
         * \return void
         */
        void setTitle (std::string title);
        /**
         * \brief Set plot style. (points, lines, etc.)
         * \param style (???) Style to be set.
         * \return void
         */
        void setStyle (std::string style);
        void plot2D(int x, int y);
        void plot3D(int x, int y, int z);
        ~Visualisation();
};

#endif
