#ifndef DATA__HPP
#define DATA__HPP

#include "Point.hpp"
#include "Statistics.hpp"
#include <vector>
#include <string>

static const std::vector<std::string> types {"data", "csv", "arff", "txt"};
enum  Type {TYPE_INVALID = -1, TYPE_DATA = 0, TYPE_CSV = 1, TYPE_ARFF = 2, TYPE_TXT = 3};

/**
 * \brief Wrapper for the dataset data.
 */
class Data {
    // Associations
    // Attributes
    public :
        /// Set of points.
        std::vector<Point> points;
        /// Features names.
        std::vector<int> fnames;
        std::vector<int> index;
        /// Number of points in the dataset.
        int size = 0;
        /// Dataset points dimension.
        int dim = 0;
        /// Verify if there's some data loaded.
        bool is_empty = true;
        /// Values for statistical methods.
        Statistics stats;
    // Operations
        /**
         * \brief Returns the type of the file.
         * \param file (???) Path to the file.
         * \return int
         */
        Type identifyFileType (std::string file);
        /**
         * \brief Load a dataset from a csv file.
         * \param path (???) Path to csv file.
         * \return bool
         */
        bool load_csv (std::string path);
        /**
         * \brief Load a dataset from a arff file.
         * \param path (???) Path to arff file.
         * \return bool
         */
        bool load_arff (std::string path);
        /**
         * \brief Load a dataset from a data file.
         * \param path (???) Path to data file.
         * \return bool
         */
        bool load_data (std::string path);
        /**
         * \brief Load a dataset from a txt file.
         * \param path (???) Path to txt file.
         * \return bool
         */
        bool load_txt (std::string path);
    public :
        /**
         * \brief Constructor for empty data.
         */
        Data ();
        /**
         * \brief Data constructor to load a dataset from a file.
         * \param dataset (???) Path to the dataset to be loaded.
         */
        Data (std::string dataset);
        /**
         * \brief Returns the size of the dataset.
         * \return int
         */
        int getSize ();
        /**
         * \brief Returns the dimension of the dataset.
         * \return int
         */
        int getDim ();
        /**
         * \brief Returns the features names.
         * \return std::vector<int>
         */
        std::vector<Point> getPoints();
        std::vector<int> getFeaturesNames ();
        Statistics getStatistics();
        /**
         * \brief Return the number of positive points.
         * \return int
         */
        int getNumberPositivePoints ();
        /**
         * \brief Return the number of negative points.
         * \return int
         */
        int getNumberNegativePoints ();
        /**
         * \brief Returns if there's a dataset loaded.
         * \return bool
         */
        bool isEmpty ();
        /**
         * \brief Load a dataset from a file.
         * \param file (???) Path to dataset file.
         * \return bool
         */
        bool load (std::string file);
        /**
         * \brief Returns a copy of the data.
         * \return Data
         */
        Data copy ();
        /**
         * \brief Remove a point from the data.
         * \param pid (???) Index of the point to be removed.
         * \return bool
         */
        bool removePoint (int pid);
        /**
         * \brief Insert a point to the data from another sample.
         * \param sample (???) Sample with the point to be added.
         * \param id (???) Index of the point to be added.
         * \return bool
         */
        bool insertPoint (Data sample, int id);
        /**
         * \brief Remove several points from the sample.
         * \param ids (???) Ids of the points to be removed (must be sorted).
         * \return bool
         */
        std::vector<bool> removePoints (std::vector<int> ids);

        void operator=(const Data&);
};

#endif
