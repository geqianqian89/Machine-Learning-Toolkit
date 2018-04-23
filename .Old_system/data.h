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

#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED
#include <vector>

using namespace std;

static const vector<string> types {"data", "csv", "arff", "txt"};
enum  Type {inv = -1, data = 0, csv = 1, arff = 2, txt = 3};

class Embrapa;

//Training points: Used to store data points
struct Point{
    vector<double> x;
    int y;
    int id;
    double original_pta, corrected_pta;
	double alpha;
	bool has_pta;
  
    Point()	: y(0), id(0), original_pta(0.0), corrected_pta(0.0), alpha(0.0), has_pta(false) {};
};

//Sammple struct: Used to store sample and feature names
struct Sample{
    vector<Point*> points;
    vector<int> fnames;
	int size;
	int dim;
	int npos_samples;
	int nneg_samples;
	bool empty;
	bool normalized;
	double bias;
    double margin;
    double norm;
	double p;
	double q;
	double max_time; //tempo maximo (IMA)
    int mult_tempo; //time multiplier (IMA)
    double kernel_param;
    int kernel_type;
    double flexible;
    double alpha_aprox;
	int* index;
	int timesPerceptron;	
	int permutation;
	int comite; //average or votation
	double similarity;
	
    Sample()	: size(0),	dim(0),	npos_samples(0), nneg_samples(0),empty(true), normalized(false), bias(0.0), margin(0.0), norm(0.0), p(0.0),	q(0.0), max_time(0.0), mult_tempo(0), kernel_param(0.0), kernel_type(0), flexible(0.0), alpha_aprox(0.0), index(NULL), timesPerceptron(0), permutation(0), comite(0), similarity(0.0) {}	
	static Sample* copy(Sample *samp);
    static Sample* copy_sample_zero(Sample *samp);
    ~Sample();
};


class Data{
 private:
 	string dataset;
    Sample *samples;
	Sample* load_data(string file);
	Sample* load_csv(string file);
	Sample* load_arff(string file);
	Sample* load_txt(string file);
	int write_routine(ofstream& outfile, Sample *sample, int sv_only);
 public:
    Data()	: samples(NULL) {}
	Type detect_file_type(string fileName);
    int load(string fileName);
	int load_weights(string fname, double **w, int dim);
    int write(string fname, Sample *sample, int sv_only);
    int write_weights(string fname, double *w, int dim);
	Sample* insert_features(Sample *samples, vector<int> ins_feat, int fsize, int verbose);
	static Sample* remove_features(Sample *samples, vector<int> rem_feat);
	Sample* insert_point(Sample* samp, Sample* samp_in, int index);
	static Sample* remove_point(Sample *samp, int ind);
    static Sample* remove_points_without_id(Sample *samples, vector<int> ids);
    Sample* part_train_test(Sample* train_sample, Sample* test_sample, int fold, unsigned int seed, int verbose);
    void remove_test_sample_features(Sample *train_sample, Sample *test_sample, int verbose);
    Sample* join_samples(Sample *sample1, Sample *sample2);
    static void normalized(double *w, double q, int dim);
	void normalize_dataset();
	double calculate_norm(double **K);
	void norm(double p);
	int generate_classes(int option);
	double get_dist_centers_without_feats(vector<int> feats, int index);
	double get_radius(int index, double q);
	double get_dist_centers(int index);
	double standart_deviation(vector<double> vet);
	double get_feature_std(int index);
	double get_variance(int index);
	double get_feature_average(int index);
	void change_x_vector(Sample *samp, int *index);
	Sample* get_samples();
	void set_samples(Sample* samples);
	int get_size();
	int get_dim();
	vector<Point*> get_points();
	vector<int> get_fnames();
	int num_positives(){ return samples->npos_samples; }
	int num_negatives(){ return samples->nneg_samples; }
	void clear();
	void set_dataset_name(string fname){ dataset = fname; }
	string get_dataset_name(){ return dataset; }
	void set_maxtime(double val);
	void set_norm(double p, double q);
	void set_norm(double p);
	void print();
	bool empty();
	bool is_normalized();
	static int stoin(string str);
	static double stodn(string str);
	static bool is_number(string str);
	~Data(){delete samples;};
};

struct train_data : public Data{
    Sample *z;
    double *func;
    double *w;
    double norm;
};

struct datadual : public train_data{
	double **K;
};

#endif 
