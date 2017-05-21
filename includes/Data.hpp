#ifndef DATA__HPP
#define DATA__HPP

#include <vector>
#include <string>
#include "Point.hpp"
#include "Statistics.hpp"

static const std::vector<std::string> types {"data", "csv", "arff", "txt"};
enum  Type {TYPE_INVALID = -1, TYPE_DATA = 0, TYPE_CSV = 1, TYPE_ARFF = 2, TYPE_TXT = 3};

/**
 * \brief Wrapper for the dataset data.
 */
class Data {
    // Associations
    // Attributes
    private :
        /// Set of points.
        std::vector<Point> points;
        /// Features names.
        std::vector<int> fnames;
        std::vector<int> index;
        /// Number of points in the dataset.
        int size = 0;
        /// Dataset points dimension.
        int dim = 0;
        std::string pos_class, neg_class;
        /// Verify if there's some data loaded.
        bool is_empty = true;
        /// Verify if the data is normalized.
        bool normalized = false;
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
         * \param pos_class String representing the positive class on the dataset.
         * \param neg_class String representing the negative class on the dataset.
         */
        Data (const char* pos_class = "1", const char* neg_class = "-1");
        /**
         * \brief Data constructor to load a dataset from a file.
         * \param dataset (???) Path to the dataset to be loaded.
         * \param pos_class String representing the positive class on the dataset.
         * \param neg_class String representing the negative class on the dataset.
         */
        Data (std::string dataset, const char* pos_class = "1", const char* neg_class = "-1");
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
         * \brief Returns the point with the given index.
         * \param index    Position of a point in the points array.
         * \return std::vector<Points>
         */
        Point getPoint (int index);
        void setPoint (int index, Point p);
        /**
         * \brief Returns the vector of Points of the sample.
         * \return std::vector<Points>
         */
        std::vector<Point> getPoints ();
        /**
         * \brief Returns the features names.
         * \return std::vector<int>
         */
        std::vector<int> getFeaturesNames ();
        /**
         * \brief Returns a class with the statistics info of the sample.
         * \return Statistics
         */
        Statistics getStatistics ();
        /**
         * \brief Returns the vector of indexes.
         * \return std::vector<int>
         */
        std::vector<int> getIndex();
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
        /*!
         * \brief setClasses Set the classes of the dataset.
         * \param pos   Positive class.
         * \param neg   Negative class.
         */
        void setClasses(std::string pos, std::string neg);
        /**
         * \brief Returns if there's a dataset loaded.
         * \return bool
         */
        bool isEmpty ();
        /**
         * \brief Returns if the dataset is normalized.
         * \return bool
         */
        bool isNormalized();
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
         * \brief Returns a copy of the data with zero points.
         * \return Data
         */
        Data copyZero ();
        /**
         * \brief Merge one dataset with another.
         * \param data (???) Dataset to be joined.
         * \return bool
         */
        void join(Data data);
        /**
         * \brief Insert a point to the data from another sample.
         * \param sample (???) Sample with the point to be added.
         * \param id (???) Index of the point to be added.
         * \return bool
         */
        bool insertPoint (Data sample, int id);
        /**
         * \brief Insert a point to the end of vector points.
         * \param p (???) Point to be inserted.
         * \return bool
         */
        bool insertPoint (Point p);
        /**
         * \brief Remove several points from the sample.
         * \param ids (???) Ids of the points to be removed (must be sorted).
         * \return booleans informing which points were removed succesfully.
         */
        std::vector<bool> removePoints (std::vector<int> ids);
        /**
         * \brief Remove a point from the data.
         * \param pid (???) Index of the point to be removed.
         * \return bool
         */
        bool removePoint (int pid);
        /**
         * \brief Remove several features from the sample.
         * \param feats (???) Names of the features to be removed (must be sorted).
         * \return boolean informing if all features were succesfully removed.
         */
        bool removeFeatures (std::vector<int> feats);
        /**
         * \brief Change the x vector of a sample.
         * \param index (???) Indexes of the change to be made.
         * \return void
         */
        void changeXVector(std::vector<int> index);
        /**
         * \brief normalize Normalize the dataset using a Lp-norm.
         * \param p Norm to be utilized.
         */
        void normalize(double p = 2);
        /**
         * \brief normalize Normalize a vector using a Lp-norm.
         * \param p Norm to be utilized.
         */
        static void normalize(std::vector<double> &p, double q);

        void operator=(const Data&);
        friend std::ostream &operator<<( std::ostream &output, const Data &data );

};

#endif
