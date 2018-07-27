#ifndef VALIDATION__HPP
#define VALIDATION__HPP

#include "Classifier.hpp"
#include "Data.hpp"
#include "ValidationSolution.hpp"

/**
 * \brief Class of methods for the validation of ML algorithms.
 */
template < typename T >
class Validation {
    // Attributes
private :
    /// Model to be validated.
    Classifier< T >  *classifier;
    /// Sample used in the validation.
    std::shared_ptr<Data< T > > sample;
    /// Train and test sample used.
    std::shared_ptr<Data< T > > train_sample, test_sample;
    double initial_error, limit_error, actual_error;
    /// Parameter used in the kernel in case of Dual classifiers.
    double kernel_param;
    /// Type of the kernel in case of Dual classifiers.
    int kernel_type;
    /// Controls the information printed in the console.
    int verbose = 0;
    /// Seed used in the random generator.
    unsigned int seed;
    /// Solution generated by the validation.
    ValidationSolution solution;

    // Operations
public :
    struct CrossValidation{
        size_t qtde = 0;
        int fold = 0;
        int jump = 0;
        std::vector<unsigned int> seed;

        double initial_error = 0.0;
        double actual_error = 0.0;
        double limit_error = 0.0;
    };

    /**
     * \brief Default constructor.
     */
    Validation ();
    /**
     * @brief Constructor initializing the sample and classifier used.
     * @param sample Sample to be used in the validation.
     * @param classifier Model to be validated.
     * @param seed  Seed to feed the pseudo random number generator.
     */
    explicit Validation (std::shared_ptr<Data< T > > sample = std::make_shared<Data< T > >(), Classifier< T >  *classifier = nullptr, unsigned int seed = 666);
    void setSamples(std::shared_ptr<Data< T > > sample);
    /**
    * \brief Set the seed for the random number generator.
    * \param seed Seed to be set.
    */
    void setSeed(unsigned int seed);
    /**
     * \brief Executes the Stratified K-fold algorithm
     * \param fold Number of folds.
     */
    void partTrainTest(int fold);
    /**
     * \brief Executes the k
     * \param fold Number of folds.
     * \param seed  Seed to feed the pseudo random number generator.
     */
    double kFold (int fold, int seed);
    /**
     * @brief Executes the validation with several executions of the k fold algorithm
     * @param fold Number of folds.
     * @param qtde Number of executions.
     * @return double Validation error.
     */
    ValidationSolution validation (int fold, int qtde);
    /**
     * @brief Get the train sample used in the validation of the model.
     * @return Data
     */
    std::shared_ptr<Data< T > > getTestSample ();
    /**
     * @brief Get the train sample used in the validation of the model.
     * @return Data
     */
    std::shared_ptr<Data< T > > getTrainSample ();
    /**
     * @brief Set the verbose.
     * @param verbose Verbose level.
     */
    void setVerbose(int verbose){this->verbose = verbose;}
};

#endif
