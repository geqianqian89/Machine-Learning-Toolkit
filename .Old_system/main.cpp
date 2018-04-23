#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <ctime>
#ifdef __unix__
	#include <dirent.h>
#elif _WIN32
	#include <windows.h>
#endif
#include "data.h"
#include "almap.h"
#include "golub.h"
#include "fisher.h"
#include "imap.h"
#include "smo.h"
#include "rfe.h"
#include "imadual.h"
#include "adaboost.h"
#include "adaboostdual.h"

using namespace std;

const string data_folder = "DB/";
int verbose;
bool sair = false;
bool inva = false;
Data* _data = NULL;
Sample *test_sample = NULL;
double max_time = 100.0f;

//Menus utilities
void clear(void);
void list_datasets(void);
bool valid_file(string file);
void waitUserAction(void);
void exitProgram(void);

//Prints that are common in all menus
void header(void);
int selector(void);

//Functions to show menus options
void mainMenu(void);
void datasetMenu(void);
void dataMenu(void);
void classifiersMenu(void);
void emsemblesMenu(void);
void featureSelectionMenu(void);
void genomicMenu(void);
void utilsMenu(void);

//Functions to execute menus options
void mainOption(int);
void classifiersOption(int);
void datasetOption(int);
void dataOption(int);
void emsemblesOption(int);
void featureSelectionOption(int);
void genomicOption(int);
void utilsOption(int);

//Feature Selection Menus
void rfe(Sample* sample, int op_rfe);
void golub(Sample* sample, int op_golub);
void fisher(Sample* sample, int op_fisher);

//Utils menus
void kfold(Sample* sample, int op_fold, int fold, int qtdcross);
void validationKfold(Sample* sample, int op_fold, int fold, int qtdcross);

int main(int argc, char** argv){
	string opt;

	_data = new Data;

	if(argc == 2) verbose = atoi(argv[1]); else verbose = 1;

	while(true){
		if(sair) exitProgram();
		clear();
		mainMenu();
	}

    return 0;
}

bool valid_file(string file){
	int i;
	bool flag = false;
	string ext;

	if(file.empty() || file.size() < 4)
		return false;

	for(i = file.size()-1; i >= 0; i--){
		if(file[i] == '.') break;
		string f(1, file[i]);
		ext = f + ext;
	}

	for(string type : types){
		if(type == ext) flag = true;
	}

	return flag;
}

void list_datasets(void){
	#ifdef __unix__
		DIR *dpdf;
		struct dirent *epdf;

		string temp = data_folder;
		temp.erase(temp.begin() + temp.size()-1);
		string path = "./" + data_folder;

		dpdf = opendir(path.c_str());
		if(dpdf != NULL){
			while((epdf = readdir(dpdf))){
				string file = string(epdf->d_name);
				if(valid_file(file) && !file.empty()){
					cout << "- " << file << endl;
				}
			}
		}

		closedir(dpdf);
	#elif _WIN32
		HANDLE hFind;
		WIN32_FIND_DATA data;

		hFind = FindFirstFile(".\\DB\\*.*", &data);
		if (hFind != INVALID_HANDLE_VALUE) {
		  do {
		  	string file_name(data.cFileName);
			if(valid_file(file_name) && !file_name.empty())
				cout << "- " << file_name << "\n";
		  } while (FindNextFile(hFind, &data));
		  FindClose(hFind);
		}
	#else
		cout << "This system is not supported for this function..." << endl;
	#endif
}

void clear(void){
#ifdef __unix__
	system("clear");
#elif _WIN32
	system("CLS");
#else
	int n;
	for (n = 0; n < 10; n++)
      printf( "\n\n\n\n\n\n\n\n\n\n" );
#endif
}

void exitProgram(void){
	if(_data)
		delete _data;
	if(test_sample)
		delete test_sample;
	exit(0);
}

void waitUserAction(void){
	cout << "\nPress ENTER to continue..." << endl;
	cin.ignore(numeric_limits<streamsize>::max(),'\n');
	cin.get();
}

void header(void){
	cout << "         *----------------------------------------------------------* " << endl;
	cout << "         *                 Machine Learning Toolkit                 * " << endl;
	cout << "         *----------------------------------------------------------* " << endl;
	cout << endl;
	cout << "Select an option:\n" << endl;
}

int selector(void){
	int o;
	string opt;

	if(inva){
		cout << endl;
		cout << "Invalid option." << endl;
		inva = false;
		cout << endl;
	}
	cout << " > ";
	cin >> opt;
	istringstream iss(opt);
	iss >> o;
	clear();
	return o;
}

void mainMenu(){
	int option;

	clear();
	header();

	cout << "1 - Dataset" << endl;
	cout << "2 - Data" << endl;
	cout << "3 - Classifiers" << endl;
	cout << "4 - Emsembles" << endl;
	cout << "5 - Feature Selection" << endl;
	cout << "6 - Genomic" << endl;
	cout << "7 - Utils" << endl;
	cout << "0 - Exit" << endl;

	option = selector();
	mainOption(option);
}

void datasetMenu(void){
	int option;

	clear();
	header();

	cout << "1 - Load a dataset" << endl;
	cout << "2 - Dataset information " << endl;
	cout << "3 - Features names" << endl;
	cout << "4 - Set max time" << endl;
	cout << "5 - Divide dataset in Train/Test" << endl;
	cout << "6 - Save Train/Test dataset" << endl;
	cout << "0 - Back to the main menu" << endl;

	option = selector();
	datasetOption(option);
}

void dataMenu(void){
	int option;

	clear();
	header();

	cout << "1 - Insert features" << endl;
	cout << "2 - Remove features" << endl;
	cout << "3 - Remove point" << endl;
	cout << "4 - Data variance" << endl;
	cout << "5 - Data radius" << endl;
	cout << "6 - Distance from the center of the classes" << endl;
	cout << "7 - Normalize dataset" << endl;
	cout << "0 - Back to the main menu" << endl;

	option = selector();
	dataOption(option);
}

void classifiersMenu(void){
	int option;

	clear();
	header();

	cout << "1 - IMAp Primal" << endl;
	cout << "2 - IMA Dual" << endl;
	cout << "3 - ALMAp" << endl;
	cout << "4 - SMO" << endl;
	cout << "0 - Back to the main menu" << endl;

	option = selector();
	classifiersOption(option);
}

void emsemblesMenu(void){
	int option;

	clear();
	header();
	
	cout << "1 - AdaBoost Primal" << endl;
	cout << "2 - AdaBoost Dual (Not working yet)" << endl;
	cout << "0 - Back to the main menu" << endl;

	option = selector();
	emsemblesOption(option);
}

void featureSelectionMenu(void){
	int option;

	clear();
	header();

	cout << "1 - RFE (Recursive Feature Elimination)" << endl;
	cout << "2 - Golub" << endl;
	cout << "3 - Fisher" << endl;
	cout << "0 - Back to the main menu" << endl;

	option = selector();
	featureSelectionOption(option);
}

void genomicMenu(void){
	int option;

	clear();
	header();

	cout << "1 - Split dataset in classes according pta values" << endl;
	cout << "0 - Back to the main menu" << endl;

	option = selector();
	genomicOption(option);
}

void utilsMenu(void){
	int option;

	clear();
	header();

	cout << "1 - K-Fold" << endl;
	cout << "2 - Validation with K-Fold" << endl;
	cout << "0 - Back to the main menu" << endl;

	option = selector();
	utilsOption(option);
}

void mainOption(int option){
	switch(option){
		case 1:
			datasetMenu();
			break;
		case 2:
			dataMenu();
			break;
		case 3:
			classifiersMenu();
			break;
		case 4:
			emsemblesMenu();
			break;
		case 5:
			featureSelectionMenu();
			break;
		case 6:
			genomicMenu();
			break;
		case 7:
			utilsMenu();
			break;
		case 0:
			exitProgram();
			break;
		default:
			inva = true;
		break;
	}
	mainMenu();
}

void datasetOption(int option){
	int i = 0;

	switch(option){
		case 1:
			if(_data == NULL || _data->empty()){
				string fileName, path;

				if(!_data){
					_data = new Data;
				}

				cout << "List all available datasets on DB folder?[y|n]" << endl;
				cout << " > ";
				cin >> fileName;

				if(fileName == "y")
					list_datasets();
				cout << endl;
				cout << "Enter the name of the DB (must be in the DB folder): ";
				cin >> fileName;

				path = data_folder + fileName;
				for(i = fileName.size() - 1; i >= 0; i--){
					if(fileName[i] == '.')
						fileName.erase(fileName.begin() + i, fileName.end());
				}

				clock_t begin = clock();
				_data->set_dataset_name(fileName);
				_data->load(path);
				clock_t end = clock();

				double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
				cout << endl;
				cout << elapsed_secs << " seconds to load.\n";
			}else{
				char y;
				cout << "Which to load a new dataset?[y|n]" << endl;
				cout << " > ";
				cin >> y;

				if(y == 'y'){
					delete _data;
					if(test_sample){
						delete test_sample;
						test_sample = NULL;
					}
					_data = NULL;
					cout << "\nOld dataset erased, select this option again."<< endl;
				}
			}

			waitUserAction();
			break;
		case 2:
			if(_data != NULL && !_data->empty()){
				cout << "Number of features: ";
				cout << _data->get_dim() << endl;
				cout << "Number of samples: ";
				cout << _data->get_size() << endl;
				cout << "Negative points: ";
				cout << _data->num_negatives() << endl;
				cout << "Positive points: ";
				cout << _data->num_positives() << endl;

				if(test_sample != NULL){
					cout << "\n\nTest sample information\n\n";
					cout << "Number of features: ";
					cout << test_sample->dim << endl;
					cout << "Number of samples: ";
					cout << test_sample->size << endl;
					cout << "Negative points: ";
					cout << test_sample->nneg_samples << endl;
					cout << "Positive points: ";
					cout << test_sample->npos_samples << endl;
				}
			}else cout << "Load a dataset first...\n\n";

			waitUserAction();
			break;

		case 3:
			if(_data != NULL && !_data->empty()){
				vector<int> fnames = _data->get_fnames();

				for(int f : fnames){
					cout << f << " : ";
				}
				cout << endl;
			}else cout << "Load a dataset first...\n\n";

			waitUserAction();
			break;
		case 4:
			cout << "Enter the max time: ";
			cin >> max_time;

			waitUserAction();
			break;
		case 5:
			if(_data != NULL && !_data->empty()){
				int fold;
				unsigned int seed;

				if(!test_sample){
					Sample *sample = Sample::copy(_data->get_samples());
					test_sample = new Sample;

					cout << "K-Fold: ";
					cin >> fold;
					cout << "Seed for timestamps: ";
					cin >> seed;

					clock_t begin = clock();
					test_sample = _data->part_train_test(sample, test_sample, fold, seed, verbose);
					clock_t end = clock();

					cout << "\nDone!" << endl;
					double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
					cout << "Size of the test sample: " << test_sample->size << endl;
					cout << endl;
					cout << elapsed_secs << " seconds to compute.\n";

				}else cout << "Test sample already divided...\n";
			}else cout << "Load a dataset first...\n\n";

			waitUserAction();
			break;
		case 6:
			if(_data != NULL && !_data->empty()){
				if(!test_sample){
					cerr << "Divide the train/test datasets first...\n" << endl;
				}else{
					string outfile = _data->get_dataset_name();
					Sample *sample = Sample::copy(_data->get_samples());

					outfile = outfile + "_train";
					_data->write(outfile, sample, 0);
					outfile = _data->get_dataset_name();
					outfile = outfile + "_test";
					_data->write(outfile, test_sample, 0);

					delete sample;
				}
			}else cout << "Load a dataset first...\n\n";

			waitUserAction();
			break;
		case 0:
			mainMenu();
			break;
		default:
			inva = true;
		break;
	}
	datasetMenu();
}

void dataOption(int option){
	int i = 0, j = 0;

	switch(option){
			case 1:
				if(_data != NULL && !_data->empty()){
					int totalFeat, flag_feat, f, fnamesize;
					vector<int> feats;
					vector<int> fnames = _data->get_fnames();

					fnamesize = fnames.size();
					cout << "Insert how many features: ";
					cin >> totalFeat;
					feats.resize(totalFeat);

					for(i = 0; i < totalFeat; i++){
						cout << "Feature " << i + 1 << ": ";
						cin >> f;
						feats[i] = f;

						for(flag_feat = 0, j = 0; j < fnamesize; j++){
							if(feats[i] == fnames[j]){
								flag_feat = 1;
							}
						}

						if(!flag_feat){
							cout << "Feature " << feats[i] << " does not belongs to the set.\n";
							i--;
							cin.clear();
						}
					}

					Sample *samples = Sample::copy(_data->get_samples());
					Sample *sample_temp = _data->insert_features(samples, feats, totalFeat, verbose);
					_data->set_samples(sample_temp);
				}else cout << "Load a dataset first...\n\n";

				waitUserAction();
				break;
		case 2:
			if(_data != NULL && !_data->empty()){
				int totalFeat, flag_feat, f, fnamesize;
				vector<int> feats;
				vector<int> fnames = _data->get_fnames();

				fnamesize = fnames.size();

				cout << "Remove how many features: ";
				cin >> totalFeat;

				for(i = 0; i < totalFeat; i++){
					cout << "Feature " << i + 1 << ": ";
					cin >> f;
					feats.push_back(f);
					for(flag_feat = 0, j = 0; j < fnamesize; j++){
						if(feats[i] == fnames[j]){
							flag_feat = 1;
						}
					}

					if(!flag_feat){
						cout << "Feature " << feats[i] << " does not belongs to the set.\n";
						i--;
					}
				}
				cout << endl;
				Sample *samples = _data->get_samples();
				_data->remove_features(samples, feats);
			}else cout << "Load a dataset first...\n\n";

			waitUserAction();
			break;
		case 3:
			if(_data != NULL && !_data->empty()){
				Sample* samp = _data->get_samples();

				cout << "Point to remove index: ";
				cin >> i;

				_data->remove_point(samp, i);
			}else cout << "Load a dataset first...\n\n";

			waitUserAction();
			break;
		case 4:
			if(_data != NULL && !_data->empty()){
				int index;

				cout << "Feature to be ignored: ";
				cin >> index;
				clock_t begin = clock();
				cout << endl;
				cout << "The variance values is: " << _data->get_variance(index) << endl;
				clock_t end = clock();

				double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
				cout << endl;
				cout << elapsed_secs << " seconds to compute.\n";
			}else cout << "Load a dataset first...\n\n";

			waitUserAction();
			break;
		case 5:
			if(_data != NULL && !_data->empty()){
				int index, q;

				cout << "Feature to be ignored: ";
				cin >> index;
				cout << "Value of q [q = 1 | q = 2]: ";
				cin >> q;
				cout << endl;

				clock_t begin = clock();
				cout << "The value of the radius is: " << _data->get_radius(index, q) << endl;
				clock_t end = clock();

				double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
				cout << endl;
				cout << elapsed_secs << " seconds to compute.\n";
			}else cout << "Load a dataset first...\n\n";

			waitUserAction();
			break;
		case 6:
			if(_data != NULL && !_data->empty()){
				int index;

				cout << "Feature to be ignored: ";
				cin >> index;
				cout << endl;

				clock_t begin = clock();
				cout << "The value of the center of the classes are: " << _data->get_dist_centers(index) << endl;
				cout << endl;
				clock_t end = clock();

				double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

				cout << elapsed_secs << " seconds to compute.\n";
			}else cout << "Load a dataset first...\n\n";

			waitUserAction();
			break;
		case 7:
			if(_data != NULL && !_data->empty()){
				int q, p;

				cout << "Value of q: ";
				cin >> q;
				cout << endl;

				if(q == -1.0){
					p = 1.0;
				}else if(q == 1.0){
					p = 100.0;
				}else{
					p = q/(q-1.0);
				}

				clock_t begin = clock();
				_data->set_norm(p, q);
				_data->normalize_dataset();
				cout << "The dataset was normalized." << endl;
				clock_t end = clock();

				double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
				cout << endl;
				cout << elapsed_secs << " seconds to compute.\n";
			}else cout << "Load a dataset first...\n\n";

			waitUserAction();
			break;
		case 0:
			mainMenu();
			break;
		default:
			inva = true;
		break;
	}
	dataMenu();
}

void classifiersOption(int option){
	switch(option){
		case 1:	//IMAp Primal
			if(_data != NULL && !_data->empty()){
				int norm;
				double p, q, flexible, alpha_prox, *w, margin;
				int svs;
				Sample* samp = Sample::copy(_data->get_samples());

				cout << "[1]p or [2]q norm: ";
				cin >> norm;
				cout << endl;

				if(norm == 1){
					cout << "p-norm value: ";
					cin >> p;
					if(p == 1.0){
						q = -1.0;
					}else{
						q = p/(p-1.0);
					}
				}else{
					cout << "q-norm value: ";
					cin >> q;
					if(q == -1.0){
						p = 1.0;
					}else if(q == 1.0){
						p = 100.0;
					}else{
						p = q/(q-1.0);
					}
				}
				cout << endl;

				cout << "Flexibilization value [0 - no flexibilization]: ";
				cin >> flexible;
				cout << endl;

				cout << "Alpha aproximation value [1 - alpha]: ";
				cin >> alpha_prox;
				cout << endl;

				w = NULL;
				margin = 0.0;
				svs = 0;

				samp->max_time = max_time;
				samp->p = p;
				samp->q = q;
				samp->kernel_type = 9;
				samp->flexible = flexible;
				samp->alpha_aprox = alpha_prox;

				clock_t begin = clock();
				if(imap(samp, &w, margin, svs, verbose)){
					cout << "Training successful..." << endl;
					cout << "\nMargin = " << margin << ", Support Vectors = " << svs << "\n" << endl;

				}
				clock_t end = clock();

				double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
				cout << endl;
				cout << elapsed_secs << " seconds to compute.\n";

				delete samp;
			}else cout << "Load a dataset first...\n\n";

			waitUserAction();
			break;
		case 2:	//IMA Dual
			if(_data != NULL && !_data->empty()){
				int kernel_type, kernel_param, svs;
				double *w, margin;

				cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
				cin >> kernel_type;

				if(kernel_type == 1){
					cout << "Polynomial degree: ";
				}else if(kernel_type == 2){
					cout << "Gaussian gamma: ";
				}

				if(kernel_type != 0)
					cin >> kernel_param;
				cin.clear();

				w = NULL;
				margin = 0.0;
				svs = 0;

				Sample *samp 	  = Sample::copy(_data->get_samples());
				samp->max_time 	  = max_time;
				samp->kernel_type = kernel_type;
				samp->kernel_param = kernel_param;

				if(imadual(samp, &w, margin, svs, verbose)){
					cout << "Training successful...\n" << endl;
					cout << "Margin = " << margin << ", Support Vectors = " << svs << endl;
				}else{
					cout << "Training failed.\n\n";
				}

				delete [] w;
				delete samp->index;
				samp->index = NULL;
			}else cout << "Load a dataset first...\n" << endl;

			waitUserAction();
			break;
		case 3:	//ALMAp
			if(_data != NULL && !_data->empty()){
				int p, svs;
				double alpha_aprox, *w, margin;

				cout << "p-norm value: ";
				cin >> p;

				cout << "Alpha aproximation value: ";
				cin >> alpha_aprox;

				w = NULL;
				margin = 0.0;
				svs = 0;

				Sample *samp 	  = Sample::copy(_data->get_samples());
				samp->p 		  = p;
				samp->max_time 	  = max_time;
				samp->kernel_type = 9;
				samp->alpha_aprox = alpha_aprox;

				clock_t begin = clock();
				if(almap(samp, &w, margin, svs, verbose)){
					cout << "Training successful...\n";
					cout << "Margin = " << margin << ", Support Vectors = " << svs << "\n";
				}else{
					cout << "Training failed.\n" << endl;
				}
				clock_t end = clock();

				double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
				cout << endl;
				cout << elapsed_secs << " seconds to compute.\n";
				delete [] w;
			}else cout << "Load a dataset first...\n" << endl;

			waitUserAction();
			break;
		case 4:
			if(_data != NULL && !_data->empty()){
				int kernel_type, kernel_param, svs;
				double *w, margin;

				cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
				cin >> kernel_type;

				if(kernel_type == 1){
					cout << "Polynomial degree: ";
				}else if(kernel_type == 2){
					cout << "Gaussian gamma: ";
				}

				if(kernel_type != 0)
					cin >> kernel_param;
				cin.clear();

				w = NULL;
				margin = 0.0;
				svs = 0;

				Sample *samp 	  = Sample::copy(_data->get_samples());
				samp->q 		  = 2;
				samp->max_time 	  = max_time;
				samp->kernel_type = kernel_type;
				samp->kernel_param = kernel_param;

				clock_t begin = clock();
				if(smo_train(samp, &w, margin, svs, verbose)){
					cout << "Training successful...\n";
					cout << "Margin = " << margin << ", Support Vectors = " << svs << "\n";
				}else{
					cout << "Training failed.\n" << endl;
				}
				clock_t end = clock();

				double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
				cout << endl;
				cout << elapsed_secs << " seconds to compute.\n";
				delete [] w;
			}else cout << "Load a dataset first...\n" << endl;

			waitUserAction();
			break;
		case 0:
			mainMenu();
			break;
		default:
			inva = true;
		break;
	}
	classifiersMenu();
}

void emsemblesOption(int option){
	int i, j, times, order = 1, kernel_type;
	double **comite = NULL, kernel_param;
	Point ***points_temp = NULL;
	vector<double> marginc, biasc, alfa;

	switch(option){
		case 0:
			mainMenu();
			break;
		case 1:	//AdaBoost Primal
			if(_data != NULL && !_data->empty()){
				Sample *sample = Sample::copy(_data->get_samples());
				
				cout << "Comite size: ";
				cin >> times;
				
				sample->p = 2;
				sample->q = 2;
				sample->max_time = max_time;
				sample->kernel_type  = 7;
                sample->flexible     = 0;
                sample->alpha_aprox  = 0;
                sample->timesPerceptron = times;
                sample->permutation = order;
				
				if(adaboost(sample, &comite, marginc, biasc, alfa, 2))
					cout << "Comite successfully trained...\n";
				else
					cout << "Training failed.\n" << endl;
			
				for(i = 0; i < sample->timesPerceptron; ++i){
					delete [] comite[i];
				}
				delete [] comite;
				marginc.clear();
				biasc.clear();
				alfa.clear();
				waitUserAction();
			}else cout << "Load a dataset first...\n\n";
			break;
		case 2:	//AdaBoost Dual
			if(_data != NULL && !_data->empty()){
				Sample *sample = Sample::copy(_data->get_samples());
				
				cout << "Comite size: ";
				cin >> times;
				cout << "\nKernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
				cin >> kernel_type;
				
				if(kernel_type == 1)
					cout << "Polynomial degree: ";
				else if(kernel_type == 2)
					cout << "Gaussian gamma: ";
				
				if(kernel_type != 0)
					cin >> kernel_param;
					
				sample->q = 2;
				sample->max_time = max_time;
				sample->kernel_type  = kernel_type;
				sample->kernel_param  = kernel_param;
                sample->flexible     = 0;
                sample->alpha_aprox  = 0;
                sample->timesPerceptron = times;
                sample->permutation = 8;
				
				if(adaboostdual(sample, &comite, marginc, biasc, alfa, &points_temp, 2))
					cout << "Comite successfully trained...\n";
				else
					cout << "Training failed.\n" << endl;
			
				for(i = 0; i < sample->timesPerceptron; ++i){
					for(j = 0; j < sample->size; ++j){
						delete points_temp[i][j];
						points_temp[i][j] = NULL;
					}
					delete [] points_temp[i];
					delete [] comite[i];
				}
				delete [] points_temp;
				delete [] comite;
				marginc.clear();
				biasc.clear();
				alfa.clear();
				waitUserAction();
			}else cout << "Load a dataset first...\n\n";
			break;
		default:
			inva = true;
		break;
	}
	emsemblesMenu();
}

void featureSelectionOption(int option){
	int rfe_opt, flag_feat, fold, golub_opt, fisher_opt;
	unsigned int seed;

	switch(option){
		case 1:
			if(_data != NULL && !_data->empty()){
				Sample *sample = Sample::copy(_data->get_samples());

				if(!test_sample){
					cout << "Split dataset in train/test? [0]No [1]Yes: ";
					cin >> flag_feat;

					if(flag_feat){
						test_sample = new Sample;

						cout << "K-Fold: ";
						cin >> fold;
						cout << "Seed for timestamps: ";
						cin >> seed;

						clock_t begin = clock();
						test_sample = _data->part_train_test(sample, test_sample, fold, seed, verbose);
						clock_t end = clock();

						cout << "\nDone!" << endl;
						double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
						cout << "Size of the test sample: " << test_sample->size << endl;
						cout << endl;
						cout << elapsed_secs << " seconds to compute.\n";
						waitUserAction();
						clear();
					}
				}

				cout << "RFE" << endl;
				cout << "1 - IMAp" << endl;
				cout << "2 - IMA Dual" << endl;
				cout << "3 - SMO" << endl;
				cout << "4 - PL" << endl;

				rfe_opt = selector();
				rfe(sample, rfe_opt);
			}else cout << "Load a dataset first...\n\n";

			waitUserAction();
			break;
		case 2:
			if(_data != NULL && !_data->empty()){
				Sample *sample = Sample::copy(_data->get_samples());

				if(!test_sample){
					cout << "Split dataset in train/test? [0]No [1]Yes: ";
					cin >> flag_feat;

					if(flag_feat){
						test_sample = new Sample;

						cout << "K-Fold: ";
						cin >> fold;
						cout << "Seed for timestamps: ";
						cin >> seed;

						clock_t begin = clock();
						test_sample = _data->part_train_test(sample, test_sample, fold, seed, verbose);
						clock_t end = clock();

						cout << "\nDone!" << endl;
						double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
						cout << "Size of the test sample: " << test_sample->size << endl;
						cout << endl;
						cout << elapsed_secs << " seconds to compute.\n";
						waitUserAction();
						clear();
					}
				}

				cout << "Golub" << endl;
				cout << "1 - IMAp" << endl;
				cout << "2 - IMA Dual" << endl;
				cout << "3 - SMO" << endl;

				golub_opt = selector();
				golub(sample, golub_opt);
			}else cout << "Load a dataset first...\n\n";

			waitUserAction();
			break;
		case 3:
			if(_data != NULL && !_data->empty()){
				Sample *sample = Sample::copy(_data->get_samples());

				if(!test_sample){
					cout << "Split dataset in train/test? [0]No [1]Yes: ";
					cin >> flag_feat;

					if(flag_feat){
						test_sample = new Sample;

						cout << "K-Fold: ";
						cin >> fold;
						cout << "Seed for timestamps: ";
						cin >> seed;

						clock_t begin = clock();
						test_sample = _data->part_train_test(sample, test_sample, fold, seed, verbose);
						clock_t end = clock();

						cout << "\nDone!" << endl;
						double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
						cout << "Size of the test sample: " << test_sample->size << endl;
						cout << endl;
						cout << elapsed_secs << " seconds to compute.\n";
						waitUserAction();
						clear();
					}
				}

				cout << "Fisher" << endl;
				cout << "1 - IMAp" << endl;
				cout << "2 - IMA Dual" << endl;
				cout << "3 - SMO" << endl;

				fisher_opt = selector();
				fisher(sample, fisher_opt);
			}else cout << "Load a dataset first...\n\n";

			waitUserAction();

			break;
		case 0:
			mainMenu();
			break;
		default:
			inva = true;
		break;
	}
	featureSelectionMenu();
}

void genomicOption(int option){
	switch(option){
		case 1:
			if(_data != NULL && !_data->empty()){
				int op_pta;

				cout << "Classes -1/+1 [1]Pos./Neg. [2]Average [3]Median [4]%% highest [5]Value: ";
				cin >> op_pta;
				cout << endl;

				clock_t begin = clock();
				_data->generate_classes(op_pta);
				clock_t end = clock();

				cout << "\nDone!" << endl;
				double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
				cout << endl;
				cout << elapsed_secs << " seconds to compute.\n";

			}else cout << "Load a dataset first...\n\n";

			waitUserAction();
			break;
		case 0:
			mainMenu();
			break;
		default:
			inva = true;
		break;
	}
	genomicMenu();
}

void utilsOption(int option){
	int qtdcross, fold, op_fold;

	switch(option){
		case 1: //K-Fold Validation
			if(_data != NULL && !_data->empty()){
				Sample *sample = Sample::copy(_data->get_samples());

				cout << "Number of K-Fold: ";
				cin >> qtdcross;

				cout << "Fold: ";
				cin >> fold;

				cout << "\n1 - IMAp" << endl;
				cout << "2 - IMA Dual" << endl;
				cout << "3 - SMO" << endl;

				op_fold = selector();
				kfold(sample, op_fold, fold, qtdcross);
			}else cout << "Load a dataset first...\n\n";

			waitUserAction();
			break;
		case 2:
			if(_data != NULL && !_data->empty()){
				Sample *sample = Sample::copy(_data->get_samples());

				cout << "Number of K-Fold: ";
				cin >> qtdcross;

				cout << "Fold: ";
				cin >> fold;

				cout << "\n1 - IMAp" << endl;
				cout << "2 - IMA Dual" << endl;
				cout << "3 - SMO" << endl;

				op_fold = selector();
				validationKfold(sample, op_fold, fold, qtdcross);
			}else cout << "Load a dataset first...\n\n";

			waitUserAction();
			break;
		case 0:
			mainMenu();
			break;
		default:
			inva = true;
		break;
	}
	utilsMenu();
}

void rfe(Sample* sample, int op_rfe){
	int jump = 0, kernel_type, dim;
	double q, flexible, alpha_aprox, kernel_param;
	string outfile;
	crossvalidation *cv = new crossvalidation;

	clear();
	switch(op_rfe){
		case 1:
			cout << "Value of the norm q: ";
			cin >> q;

			sample->kernel_type = 9;

			cout << "Flexibilization value [0 - no flexibilization]: ";
			cin >> flexible;

			cout << "Alpha aproximation value: ";
			cin >> alpha_aprox;

			sample->flexible = flexible;
			sample->alpha_aprox = alpha_aprox;

			break;
		case 4:
			q = 1;
			sample->kernel_type = 9;
			break;
		default:
			q = 2;

			cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
			cin >> kernel_type;

			if(kernel_type == 1)
				cout << "Polynomial degree: ";
			else if(kernel_type == 2)
				cout << "Gaussian gamma: ";

			if(kernel_type != 0)
				cin >> kernel_param;
			break;
	}
	cout << "Desired dimension (max. " << sample->dim << "): ";
	cin >> dim;
	cout << "Eliminated features by time: ";
	cin >> jump;

	cv->qtd = 0;

	sample->q 			 = q;
	sample->kernel_type  = kernel_type;
	sample->kernel_param = kernel_param;
	sample->max_time 	 = max_time;
	clear();

	switch(op_rfe){
		case 1: //RFE IMAp
			outfile = "rfe_imap_" + _data->get_dataset_name();

			sample = rfe_select_features(outfile, sample, imap, sample->dim - dim, jump, 0, 1, cv, 2);
			break;
		case 2: //RFE IMA Dual
			outfile = "rfe_imad_" + _data->get_dataset_name();

			sample = rfe_select_features(outfile, sample, imadual, sample->dim - dim, jump, 0, 1, cv, 2);
			break;
		case 3: //RFE SMO
			outfile = "rfe_smo_" + _data->get_dataset_name();

			sample = rfe_select_features(outfile, sample, smo_train, sample->dim - dim, jump, 0, 1, cv, verbose);
			break;
		case 4: //Linear Programming
			cout << "Not implemented yet..." << endl;
			break;
		default:
			cout << "Invalid option!" << endl;
			break;
	}
}

void golub(Sample* sample, int op_golub){
	int kernel_type, dim;
	double q, flexible, alpha_aprox, kernel_param;
	string outfile;

	clear();
	switch(op_golub){
		case 1:
			cout << "Value of the norm q: ";
			cin >> q;

			sample->kernel_type = 9;

			cout << "Flexibilization value [0 - no flexibilization]: ";
			cin >> flexible;

			cout << "Alpha aproximation value: ";
			cin >> alpha_aprox;

			sample->flexible = flexible;
			sample->alpha_aprox = alpha_aprox;

			break;
		case 4:
			q = 1;
			sample->kernel_type = 9;
			break;
		default:
			q = 2;

			cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
			cin >> kernel_type;

			if(kernel_type == 1)
				cout << "Polynomial degree: ";
			else if(kernel_type == 2)
				cout << "Gaussian gamma: ";

			if(kernel_type != 0)
				cin >> kernel_param;
			break;
	}
	cout << "Desired dimension (max. " << sample->dim << "): ";
	cin >> dim;

	sample->q 			 = q;
	sample->kernel_type  = kernel_type;
	sample->kernel_param = kernel_param;
	sample->max_time 	 = max_time;
	clear();

	switch(op_golub){
		case 1: //Golub IMAp
			outfile = "golub_imap_" + _data->get_dataset_name();

			sample = golub_select_features(outfile, sample, imap, dim, 2);
			break;
		case 2: //Golub IMA Dual
			outfile = "golub_imad_" + _data->get_dataset_name();

			sample = golub_select_features(outfile, sample, imadual, dim, 2);
			break;
		case 3: //Golub SMO
			outfile = "golub_smo_" + _data->get_dataset_name();

			sample = golub_select_features(outfile, sample, smo_train, dim, 2);
			break;
		default:
			cout << "Invalid option!" << endl;
			break;
	}
}

void fisher(Sample* sample, int op_fisher){
	int kernel_type, dim;
	double q, flexible, alpha_aprox, kernel_param;
	string outfile;

	clear();
	switch(op_fisher){
		case 1:
			cout << "Value of the norm q: ";
			cin >> q;

			sample->kernel_type = 9;

			cout << "Flexibilization value [0 - no flexibilization]: ";
			cin >> flexible;

			cout << "Alpha aproximation value: ";
			cin >> alpha_aprox;

			sample->flexible = flexible;
			sample->alpha_aprox = alpha_aprox;

			break;
		case 4:
			q = 1;
			sample->kernel_type = 9;
			break;
		default:
			q = 2;

			cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
			cin >> kernel_type;

			if(kernel_type == 1)
				cout << "Polynomial degree: ";
			else if(kernel_type == 2)
				cout << "Gaussian gamma: ";

			if(kernel_type != 0)
				cin >> kernel_param;
			break;
	}
	cout << "Desired dimension (max. " << sample->dim << "): ";
	cin >> dim;

	sample->q 			 = q;
	sample->kernel_type  = kernel_type;
	sample->kernel_param = kernel_param;
	sample->max_time 	 = max_time;
	clear();

	switch(op_fisher){
		case 1: //Fisher IMAp
			outfile = "fisher_imap_" + _data->get_dataset_name();

			sample = fisher_select_features(outfile, sample, imap, dim, 2);
			break;
		case 2: //Fisher IMA Dual
			outfile = "fisher_imad_" + _data->get_dataset_name();

			sample = fisher_select_features(outfile, sample, imadual, dim, 2);
			break;
		case 3: //Fisher SMO
			outfile = "fisher_smo_" + _data->get_dataset_name();

			sample = fisher_select_features(outfile, sample, smo_train, dim, 2);
			break;
		default:
			cout << "Invalid option!" << endl;
			break;
	}
}


void kfold(Sample* sample, int op_fold, int fold, int qtdcross){
	int norm, i, kernel_type;
	double p, q, flexible, alpha_aprox, errocross, kernel_param;
	Data data_op;
	vector<double> errocrossVet;
	clear();

	switch(op_fold){
		case 1:	//IMAp
			cout << "Norm [1]p or [2]q: ";
			cin >> norm;

			if(norm == 1){
				cout << "Value of the p-norm: ";
				cin >> p;

				if(p == 1.0)
					q = -1.0;
				else q = p/(p-1.0);
			}else{
				cout << "Value of the q-norm: ";
				cin >> q;

				if(q == 1.0)
					p = 100.0;
				else p = q/(q-1.0);
			}

			cout << "Value of the flexibilization [0 - no flexibilization]: ";
			cin >> flexible;

			cout << "Value of the alpha aproximation [1 - alpha]: ";
			cin >> alpha_aprox;

			sample->p 			= p;
			sample->q 			= q;
			sample->max_time 	= max_time;
			sample->kernel_type = 9;
			sample->flexible 	= flexible;
			sample->alpha_aprox = alpha_aprox;

			errocrossVet.resize(qtdcross);

			for(errocross = 0, i = 0; i < qtdcross; i++){
				if(verbose){
					cout << "\nExecution " << i+1 << " / " << qtdcross << ": \n";
					errocrossVet[i] = utils_k_fold(sample, imap, fold, i, verbose);
					errocross += errocrossVet[i];
					cout << "\nExecution Error " << i+1 << " / " << qtdcross << ": " << errocrossVet[i] << endl;
				}

				cout << "Average Error " << fold << "- Fold Cross Validation: " << errocross/qtdcross << " " << 241 << " " << _data->standart_deviation(errocrossVet) << endl;
			}

			break;
		case 2: //IMA Dual
			cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
			cin >> kernel_type;

			if(kernel_type == 1){
				cout << "Polinomial degree: ";
			}else if(kernel_type == 2){
				cout << "Gaussian gamma: ";
			}

			if(kernel_type){
				cin >> kernel_param;
			}

			sample->q = 2;
			sample->max_time = max_time;
			sample->kernel_param = kernel_param;

			errocrossVet.resize(qtdcross);

			for(errocross = 0, i = 0; i < qtdcross; i++){
				if(verbose){
					cout << "\nExecution " << i+1 << " / " << qtdcross << ": \n";
					errocrossVet[i] = utils_k_fold(sample, imadual, fold, i, verbose);
					errocross += errocrossVet[i];
					cout << "\nExecution Error " << i+1 << " / " << qtdcross << ": " << errocrossVet[i] << endl;
				}

				cout << "Average Error " << fold << "- Fold Cross Validation: " << errocross/qtdcross << " " << 241 << " " << _data->standart_deviation(errocrossVet) << endl;
			}


			break;
		case 3:	//SMO
			cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
			cin >> kernel_type;

			if(kernel_type == 1){
				cout << "Polinomial degree: ";
			}else if(kernel_type == 2){
				cout << "Gaussian gamma: ";
			}

			if(kernel_type){
				cin >> kernel_param;
			}

			sample->q = 2;
			sample->max_time = max_time;
			sample->kernel_param = kernel_param;

			errocrossVet.resize(qtdcross);

			for(errocross = 0, i = 0; i < qtdcross; i++){
				if(verbose){
					cout << "\nExecution " << i+1 << " / " << qtdcross << ": \n";
					errocrossVet[i] = utils_k_fold(sample, smo_train, fold, i, verbose);
					errocross += errocrossVet[i];
					cout << "Execution Error " << i+1 << " / " << qtdcross << ": " << errocrossVet[i] << endl;
				}

				cout << "Average Error " << fold << "- Fold Cross Validation: " << errocross/qtdcross << " " << 241 << " " << _data->standart_deviation(errocrossVet) << endl;
			}

			break;
		default:

			break;
	}
}

void validationKfold(Sample* sample, int op_fold, int fold, int qtdcross){
	int norm, kernel_type;
	double p, q, flexible, alpha_aprox, kernel_param;

	if(!test_sample){
		test_sample = _data->part_train_test(sample, test_sample, fold, 0, verbose);
	}else if(test_sample->dim > sample->dim){
		_data->remove_test_sample_features(sample, test_sample, verbose);
	}

	switch(op_fold){
		case 1: //IMAp
			cout << "Norm [1]p or [2]q: ";
			cin >> norm;

			if(norm == 1){
				cout << "Value of the p-norm: ";
				cin >> p;

				if(p == 1.0)
					q = -1.0;
				else q = p/(p-1.0);
			}else{
				cout << "Value of the q-norm: ";
				cin >> q;

				if(q == 1.0)
					p = 100.0;
				else p = q/(q-1.0);
			}

			cout << "Value of the flexibilization [0 - no flexibilization]: ";
			cin >> flexible;

			cout << "Value of the alpha aproximation [1 - alpha]: ";
			cin >> alpha_aprox;

			sample->p 			= p;
			sample->q 			= q;
			sample->max_time 	= max_time;
			sample->kernel_type = 9;
			sample->flexible 	= flexible;
			sample->alpha_aprox = alpha_aprox;

			utils_validation(sample, test_sample, imap, fold, qtdcross, verbose);
			break;
		case 2:	//IMA Dual
			cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
			cin >> kernel_type;

			if(kernel_type == 1){
				cout << "Polinomial degree: ";
			}else if(kernel_type == 2){
				cout << "Gaussian gamma: ";
			}

			if(kernel_type){
				cin >> kernel_param;
			}

			sample->q = 2;
			sample->max_time = max_time;
			sample->kernel_param = kernel_param;

			utils_validation(sample, test_sample, imadual, fold, qtdcross, verbose);
			break;
		case 3:	//SMO
			cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
			cin >> kernel_type;

			if(kernel_type == 1){
				cout << "Polinomial degree: ";
			}else if(kernel_type == 2){
				cout << "Gaussian gamma: ";
			}

			if(kernel_type){
				cin >> kernel_param;
			}

			sample->q = 2;
			sample->max_time = max_time;
			sample->kernel_param = kernel_param;

			utils_validation(sample, test_sample, smo_train, fold, qtdcross, verbose);
			break;
	}
}
