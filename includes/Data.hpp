/*
                       _oo0oo_
                      o8888888o
                      88" . "88
                      (| -_- |)
                      0\  =  /0
                    ___/`---'\___
                  .' \\|     |// '.
                 / \\|||  :  |||// \
                / _||||| -:- |||||- \
               |   | \\\  -  /// |   |
               | \_|  ''\---/''  |_/ |
               \  .-\__  '-'  ___/-. /
             ___'. .'  /--.--\  `. .'___
          ."" '<  `.___\_<|>_/___.' >' "".
         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
         \  \ `_.   \_ __\ /__ _/   .-` /  /
     =====`-.____`.___ \_____/___.-`___.-'=====
                       `=---='


     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

           A Buddha statue to bless your
               code to be bug free
*/
#ifndef DATA__HPP
#define DATA__HPP
#pragma pack(1)
#include <vector>
#include <string>
#include <memory>

#include "Point.hpp"
#include "Statistics.hpp"

static const std::vector<std::string> types {"data", "csv", "arff", "txt", "plt"};
enum  Type {TYPE_INVALID = -1, TYPE_DATA = 0, TYPE_CSV = 1, TYPE_ARFF = 2, TYPE_TXT = 3};

/**
 * \brief Wrapper for the dataset data.
 */
class Data {
    // Associations
    // Attributes
    private :
        /// Set of points.
        std::vector<std::shared_ptr<Point> > points;
        /// Features names.
        std::vector<int> fnames;
        /// Points indexes.
        std::vector<int> index;
        /// Number of points in the dataset.
        int size = 0;
        /// Dataset points dimension.
        int dim = 0;
        /// Positive and negative classes. (1, -1 are the default classes)
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
         * \brief write Write the data to a file with the given extention.
         * \param fname Name of the file.
         * \param ext   Extention of the file.
         */
        void write(std::string fname, std::string ext);
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
         * \brief setDim Set the dimension of the points.
         * \param dim (???) Dimension to be set.
         */
        void setDim(int dim);
        /**
         * \brief Returns the point with the given index.
         * \param index    Position of a point in the points array.
         * \return std::vector<Points>
         */
        std::shared_ptr<Point> getPoint (int index);
        /**
         * \brief setPoint Set the point in a position of the data.
         * \param index (???) Index of the point that will be set.
         * \param p (???) Point to be set.
         */
        void setPoint (int index, std::shared_ptr<Point> p);
        /**
         * \brief Returns the vector of Points of the sample.
         * \return std::vector<Points>
         */
        std::vector<std::shared_ptr<Point> > getPoints ();
        /**
         * \brief Returns the features names.
         * \return std::vector<int>
         */
        std::vector<int> getFeaturesNames ();
        /**
         * \brief setFeaturesNames Set the name of the features of the data.
         * \param fnames (???) Name of the features.
         */
        void setFeaturesNames(std::vector<int> fnames);
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
        void setIndex(std::vector<int> index);
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
         * \brief clear Clear the data.
         */
        void clear();
        /**
         * \brief Returns a copy of the data.
         * \return Data
         */
        Data copy ();
        /**
         * \brief Returns a copy of the data with zero points.
         * \return Data
         */
        void copyZero (const Data& other);
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
        bool insertPoint (std::shared_ptr<Point> p);
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
         * @brief insertFeatures Returns Data object with only features in array.
         * @param ins_feat (???) Array with features that will be in the Data object.
         * @return Data If the object is empty something wrong happened.
         */
        Data insertFeatures(std::vector<int> ins_feat);
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
         * \param q Norm to be utilized.
         * \param p Vector to be normalized.
         */
        static void normalize(std::vector<double> &p, double q);

        /*********************************************
         *  Overloaded operators for the Data class. *
         *********************************************/

        void operator=(const Data&);
        friend std::ostream &operator<<( std::ostream &output, const Data &data );

        ~Data();
};

#endif
