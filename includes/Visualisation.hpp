#ifndef VISUALISATION__HPP
#define VISUALISATION__HPP

#include "Data.hpp"
#include "Solution.hpp"
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
        /**
         * \brief Create temporary files to plot the negative and positive samples.
         * \return void
         */
        void createPosNegTemps();
        bool valid_file(std::string file);
        std::vector<std::string> getTempFilesNames();
        void removeTempFiles();
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
        /**
         * \brief Plot the selected features in 2D.
         * \param x (???) Feature to be used in the x-axis.
         * \param y (???) Feature to be used in the y-axis.
         * \return void
         */
        void plot2D(int x, int y);
        /**
         * \brief Plot the selected features in 3D.
         * \param x (???) Feature to be used in the x-axis.
         * \param y (???) Feature to be used in the y-axis.
         * \param z (???) Feature to be used in the z-axis.
         * \return void
         */
        void plot3D(int x, int y, int z);
        void plot2DwithHyperplane(int x, int y, Solution w);
        void plot3DwithHyperplane(int x, int y, int z, Solution w);
        ~Visualisation();
};

#endif
