#ifndef CLASSIFIER__HPP
#define CLASSIFIER__HPP

#include "../includes/Solution.hpp"
#include "../includes/Data.hpp"
#include "../includes/Timer.hpp"

template < typename T >
class Classifier {
    // Attributes
protected :
    /// Inform if there's an initial solution.
    bool hasInitialSolution = false;
    /// Samples used in the model training.
    std::shared_ptr<Data< T > > samples;
    /// Support vectors points.
    std::vector<Point< T > > svs;
    /// Classifier solution.
    Solution solution;
    /// Learning rate
    double rate = 0.5f;
    double gamma = 0;
    /// Initial time.
    double start_time = 0.0f;
    /// Maximum time of training.
    double max_time = 110;
    /// Number of steps in the data.
    int steps = 0;
    /// Number of updates of the weights.
    int ctot = 0;
    /// Max precision.
    double EPS = 1E-9;
    double MIN_INC = 1.001;
    /// Max number of iterations.
    int MAX_IT = 1E9;
    /// Max number of updates.
    int MAX_UP = 1E9;
    int MAX_EPOCH = 9999;
    int verbose = 1;
    /// Timer used to measure the time elapsed in the execution of classifier.
    Timer timer;
    // Operations
public :
    /**
     * @brief Returns the type of the classifier.
     * @return std::string
     */
    virtual std::string classifierType() = 0;
    /**
     * \brief Function that execute the training phase of a classification algorithm.
     * \return void
     */
    virtual bool train () = 0;
    /**
     * \brief Returns the class of a feature point based on the trained classifier.
     * \param Point< T >  x (???) Features point to be evaluated.
     * \return int
     */
    virtual double evaluate (Point< T > p) = 0;
    /**
     * \brief setSamples Set the samples used in the classifier.
     * \param samples Samples to be used.
     */
    void setSamples(std::shared_ptr<Data< T > > samples) {this->samples = samples;}
    /**
     * \brief setTimer Set the timer used in the classifier.
     * \param timer Timer to be used.
     */
    void setTimer(Timer timer) { this->timer = timer; }
    /**
     * @brief Get the elapsed time in the execution of the classifier.
     * @return double
     */
    inline double getElapsedTime() const { return timer.Elapsed(); }
    /**
    * @brief Get the total number of updates of the classifier.
    * @return int
    */
    inline int getCtot() const { return ctot; }
    /**
     * \brief getSteps Returns the number of steps through the data by the classifier.
     * \return int
     */
    inline int getSteps() const { return steps; }
    /**
     * \brief getUpdates Returns the number of updates needed to get to the the solution.
     * \return int
     */
    inline int getUpdates() const { return ctot; }
    /**
     * \brief getMaxTime Returns the maximum running time of the classifier.
     * \return double
     */
    inline double getMaxTime() const { return max_time; }
    /**
     * @brief Set the partial number of steps of the classifier.
     * @param steps Number of steps.
     */
    inline void setSteps(int steps) { this->steps = steps; }
    /**
     * @brief Set the margin of the classifier.
     * @param gamma margin.
     */
    inline void setGamma(double gamma) { this->gamma = gamma;}
    /**
     * @brief Set the partial number of updates of the classifier.
     * @param ctot Number of updates.
     */
    void setCtot(int ctot) {this->ctot = ctot;}
    /**
     * @brief Set the level of verbose.
     * @param verbose level of verbose.
     */
    void setVerbose(int verbose) {this->verbose = verbose;}
    /**
     * \brief setStartTime Set the initial time of the classifier.
     * \param start_time Initial time.
     */
    void setStartTime(double start_time) { this->start_time = start_time;}
    /**
     * \brief setMaxTime Set the maximum time of the classifier.
     * \param max_time Maximum time.
     */
    void setSolution(Solution solution) {this->solution = solution;}
    /**
     * @brief getSolution Returns the solution of the classifier.
     * @return Solution
     */
    Solution getSolution() {return this->solution;}
    /**
     * @brief getSolutionRef Returns a pointer to the solution of the classifier.
     * @return Solution
     */
    Solution *getSolutionRef() { return &this->solution; }
    /**
     * @brief Set the max time of execution.
     * @param max_time  Max time.
     */
    void setMaxTime(double max_time) {this->max_time = max_time;}
    /**
     * \brief setEPS Set the precision of the classifier.
     * \param EPS Precision.
     */
    void setEPS(double EPS) {this->EPS = EPS;}
    /**
     * \brief setMaxIterations Set the max number of iterations of the classifier.
     * \param MAX_IT Number max of iterations.
     */
    void setMaxIterations(int MAX_IT) {this->MAX_IT = MAX_IT;}
    /**
     * \brief setMaxIterations Set the max number of updates of the classifier.
     * \param MAX_IT Number max of updates.
     */
    void setMaxUpdates(int MAX_UP) {this->MAX_UP = MAX_UP;}
    /**
     * @brief Set the learning rate of the classifier.
     * @param rate Learning rate.
     */
    void setLearningRate(double rate) {this->rate = rate;}
};

#endif
