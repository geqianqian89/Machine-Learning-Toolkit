#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <iterator>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdio>

#include "data.h"
#include "embrapa.h"

/*----------------------------------------------------------*
 * Verify if the string is a number      			        *
 *----------------------------------------------------------*/

bool Data::is_number(string str){
	int i = 0, size = str.size();
	
	if(int(str[i]) == 13) return false;
	
	for(i = 0; i < size; ++i){
		if(int(str[i]) < int('0') && int(str[i]) > int('9')){
			if(str[i] != '.' && str[i] != '-')	
				return false;
		}
	}
	
	return true;
}


/*----------------------------------------------------------*
 * Convert string to integer            			        *
 *----------------------------------------------------------*/

int Data::stoin(string str){
	int i = 1, m = 1, num = 0, sign = 0, size = str.size();
	
	if(str[0] == '0'){
		return 0;	
	}else if(str[0] == '-'){
		sign = 1;
	}
	
	for(i = i + sign; i < size; ++i)
		m *= 10;
	
	for(i = sign; i < size; ++i){
		num += int(str[i] - '0')*m;
		m /= 10;
	}

	return (sign) ? num * -1 : num;
}

/*----------------------------------------------------------*
 * Convert string to double             			        *
 *----------------------------------------------------------*/

double Data::stodn(string str){
	double num, f = 0.1;
	int i = 1, m = 1, j = 0, sign = 0, size = str.size();

	if(str[0] == '-'){
		sign = 1;
	}
	
	for(i = i + sign; i < size; ++i){
		if(str[i] == '.') break;
		m *= 10;
	}
	
	for(i = sign; i < size; ++i){
		if(str[i] == '.'){
			i++;
			break;
		}
		num += int(str[i] - '0')*m;
		m /= 10;
	}

	for(j = i; j < size; ++j){
		if(str[j] >= int('0') && str[j] <= int('9')){
			num += int(str[j] - '0')*f;
			f *= 0.1;
		}
	}
	
	return (sign) ? num * -1 : num;
}

/*----------------------------------------------------------*
 * Returns the type of the file							    *
 *----------------------------------------------------------*/

Type Data::detect_file_type(string fileName){
	int l , k, i, j, tsize = types.size();

	for(k = fileName.size() - 1; (k >= 0 && fileName[k] != '.') ; k--){
		for(i = 0; i < tsize; ++i){
			if(fileName[k] == types[i][types[i].size()-1]){
				for(j = types[i].size() - 1, l = 0; (j >= 0 && fileName[k-l] != '.') ; j--, l++){
					if(j == 0 && types[i][j] == fileName[k-l]){
						switch(i){
							case 0:
									return Type::data;
									break;
							case 1:
									return Type::csv;
									break;
							case 2:
									return Type::arff;
									break;
							case 3:
									return Type::txt;
									break;
							default:
									return Type::inv;
									break;
						}
					}else if(types[i][j] != fileName[k-l]){
						break;
					}
				}
			}
		}
	}

	return Type::inv;
}

/*----------------------------------------------------------*
 * Reads data from file (svm-light format).                 *
 *----------------------------------------------------------*/

int Data::load(string fileName){
	string ptaFile;
	vector<string>::const_iterator itr;

    if(samples == NULL){
   		samples = new Sample;
   	}

    Type t = detect_file_type(fileName);

    switch(t){
        case Type::data:
        			samples = load_data(fileName);
        			break;
        case Type::csv:
        			samples = load_csv(fileName);
        			break;
        case Type::arff:
        			samples = load_arff(fileName);
        			break;
        case Type::txt:
    				samples = load_txt(fileName);
    				cout << "PTA file name: ";
					cin >> ptaFile;
					ptaFile = "DB/" + ptaFile;
    				Embrapa::load_pta(samples, ptaFile);
    				samples = Embrapa::remove_points_without_pta(samples,-1);
    				samples = Embrapa::correct_pta(samples, 2);
    				break;
        case Type::inv:
    				cout << "\nInvalid extension, the supported extensions are: ";
    				
    				for(itr = types.begin(); itr != types.end(); itr++){
    					cout << *(itr);
    					if(itr != types.end()-1) cout << ", "; else cout << ".\n";
    				}
        default:
        		return 0;
        		break;
    }
    return 1;
}

/*----------------------------------------------------------*
 * Load data from .data file						        *
 *----------------------------------------------------------*/

Sample* Data::load_data(string file){
	ifstream input(file.c_str());
	string str, item, pos_class, neg_class;
	int dim, ldim, c, size;
	bool flag;
   	Sample *samples = new Sample;

   	if(!input){
        cout << "File could not be opened!" << endl;
    	return NULL;
   	}
   	
   	cout << "Positive class: ";
	cin >> pos_class;
	cout << "Negative class: ";
	cin >> neg_class;

   	dim = ldim = size = c = 0;
   	flag = false;

   	//get dimension of the points
   	while(getline(input, str)){
   		stringstream ss(str);
   		dim = -1;

   		while(getline(ss, item, ' ')){
   			if(!is_number(item)){
   				clog << "Warning: point[" << size  << "] " << dim+1 << " feature is not a number." << endl;
   				dim--;
   			}

   			dim++;
   		}

   		if(ldim != dim && ldim != 0){
			cerr << "All the samples must have the same dimension!" << endl;
			delete samples;
			return NULL;
		}

		ldim = dim;
   		size++;
   	}

   	input.clear();
   	input.seekg(0, ios::beg);

   	//initialize dim and size
	samples->dim = dim;
	samples->size = size;

	//reserve memory for fnames array and set feature names
	samples->fnames.assign(dim, 0);
	iota(samples->fnames.begin(), samples->fnames.end(), 1);

	//reserve memory for points array
	samples->points.assign(size, NULL);

   	size = 0;

   	//get lines from file
	while(getline(input, str)){
		Point *new_point = new Point;
		stringstream ss(str);

		dim = -1;
		new_point->x.resize(samples->dim, 0.0);

		//Read features from line
		while(getline(ss, item, ' ')){
			if(dim == -1){
				c = (item == pos_class)?1:-1;
				new_point->y = c;
				if(c == -1) samples->nneg_samples++; else samples->npos_samples++;
				dim++;
			}else{
				string buffer;

				flag = false; //Verify if it's including value or fname

				for(char i: item){	//Get feature name and value
					if(i != ' ' && i != '\n'){
						if(i != ':' && !flag){
							buffer.push_back(i);
						}else if(flag){
							buffer.push_back(i);
						}else if(i == ':' && i > 0 && samples->size == 0){	//Get features names in the first running
							samples->fnames[dim] = stoin(buffer);
							flag = true;
							buffer.clear();
						}else if(i == ':'){
							buffer.clear();
							flag = true;
						}
					}
				}

				if(is_number(buffer)){
					new_point->x[dim] = stodn(buffer);
					dim++;
				}
			}
		}
		samples->points[size++] = new_point;
		new_point->id = size;
	}

	samples->empty = false;
	input.close();

	return samples;
}

/*----------------------------------------------------------*
 * Load data from .csv file							        *
 *----------------------------------------------------------*/

Sample* Data::load_csv(string file){
	ifstream input(file.c_str());
	string str, item, pos_class, neg_class;
	int dim, ldim, size, ssize;
	char deli;
	bool cond, flag, atEnd;
	Sample *samples = NULL;

	atEnd = flag = false;
	dim = ldim = size = 0;
	samples = new Sample;

	if(!input){
        cout << "File could not be opened!" << endl;
    	return NULL;
   	}

	cout << "Positive class: ";
	cin >> pos_class;
	cout << "Negative class: ";
	cin >> neg_class;

	//Verify if the class is at the begining or at the end and error check
	while(getline(input, str)){
		istringstream ss(str);
		ssize = str.size();
		dim = -1;

		//Define csv file delimitator
		if(size == 0){
			for(int i = 0; i < ssize; ++i){
				if(str[i] == ','){
					deli = ',';
					break;
				}else if(str[i] == ';'){
					deli = ';';
					break;
				}
			}
		}

		while(getline(ss, item, deli)){
			//check for invalid feature or class
			if(!is_number(item) && (item != pos_class || item != neg_class)){
   				clog << "Warning: point[" << size  << "] " << dim+1 << " feature is not a number." << endl;
   				dim--;
   			}

			if(dim == -1 && !flag){
				if(!((item == pos_class) || (item == neg_class))){
					atEnd = true;
					flag = true;
				}
			}else if(ss.eof() && !flag){
				if(!((item == pos_class) || (item == neg_class))){
					flag = true;
				}
			}
 			dim++;
		}

		if(ldim != dim && ldim != 0){
			cerr << "All the samples must have the same dimension!" << endl;
			delete samples;
			return NULL;
		}

		ldim = dim;
		size++;
	}

	input.clear();
	input.seekg(0, ios::beg);

	//initialize dim and size
	samples->dim = dim;
	samples->size = size;

	//reserve memory for fnames array and set feature names
	samples->fnames.assign(dim, 0);
	iota(samples->fnames.begin(), samples->fnames.end(), 1);

	//reserve memory for points array
	samples->points.assign(size, NULL);

	size = 0;

	//Read sample (line) from file
	while(getline(input, str)){
		Point *new_point = new Point;
		istringstream ss(str);

		dim = -1;

		//reserve memory for x array
		new_point->x.resize(samples->dim, 0.0);

		//Read features from line
		while(getline(ss, item, deli)){
			if(atEnd)
				cond = (!ss.eof() && atEnd);
			else
				cond = !(dim == -1);

			if(cond){
				if(is_number(item))
					new_point->x[dim] = stodn(item);
			}else{
				int c;
				if(is_number(item)){
					c = (item == pos_class)?1:-1;
				}else{
					c = (item == pos_class)?1:-1;
					if(item != pos_class){
						clog << "Warning: point " << size << " class is not a number and it's not defined." << endl;
						c = 0;
					}
				}
				if(c == -1) samples->nneg_samples++; else samples->npos_samples++;
				new_point->y = c;
			}
			dim++;
		}

		samples->points[size++] = new_point;
		new_point->id = size;
	}

	samples->empty = false;
	input.close();

	return samples;
}

/*----------------------------------------------------------*
 * Load data from .arff file							        *
 *----------------------------------------------------------*/

Sample* Data::load_arff(string file){
	ifstream input(file.c_str());
	string str, item, pos_class, neg_class;
	int dim, ldim, size, c;
	bool atEnd, atBegin, flag, cond;
	Sample *samples = new Sample;

	if(!input){
        cout << "File could not be opened!" << endl;
    	return NULL;
   	}

	cout << "Positive class: ";
	cin >> pos_class;
	cout << "Negative class: ";
	cin >> neg_class;

	dim = ldim = size = c = 0;
	atEnd = atBegin = flag = cond = false;

	//Verify if the class is at the begining or at the end and error check
	while(getline(input, str)){
		istringstream ss(str);
		dim = 0;

		while(getline(ss, item, ',')){
			//check for invalid feature or class
			if(!is_number(item) && (item != pos_class || item != neg_class)){
   				clog << "Warning: point[" << size  << "] " << dim+1 << " feature is not a number." << endl;
   				dim--;
   			}

			if(dim == 0 && !flag){
				if(!((item == pos_class) || (item == neg_class))){
					atEnd = true;
					flag = true;
				}
			}else if(ss.eof() && !flag){
				if(!((item == pos_class) || (item == neg_class))){
					atBegin = true;
					flag = true;
				}
			}
 			dim++;
		}

		if(ldim != dim && ldim != 0){
				cerr << "All the samples must have the same dimension!" << endl;
				delete samples;
				return NULL;
		}

		ldim = dim;
		size++;
	}

	input.clear();
	input.seekg(0, ios::beg);

	//initialize dim and size
	samples->dim = dim;
	samples->size = size;

	//reserve memory for fnames array and set feature names
	samples->fnames.assign(dim, 0);
	iota(samples->fnames.begin(), samples->fnames.end(), 1);

	//reserve memory for points array
	samples->points.assign(size, NULL);

	size = 0;

	//Read line (sample) from file
	while(getline(input, str)){
		istringstream ss(str);
		Point *new_point = new Point;
		dim = -1;

		//reserve memory for features
		new_point->x.assign(samples->dim, 0.0);

		//Read features from line
		while(getline(ss, item, ',')){
			if(atEnd)
				cond = (!ss.eof() && atEnd);
			else
				cond = !(dim == 0);
			if(cond){
				if(is_number(item))
					new_point->x[dim + 1] = stodn(item);
			}else{
				if(is_number(item)){
					c = (stoin(item) == stoin(pos_class))?1:-1;
				}else{
					c = (item == pos_class)?1:-1;
					if(item != pos_class){
						clog << "Warning: point " << size << " class is not a number and it's not defined." << endl;
						c = 0;
					}
				}
				if(c == -1) samples->nneg_samples++; else samples->npos_samples++;
				new_point->y = c;
			}
			dim++;
		}
		samples->points[size++] = new_point;
		new_point->id = size;
	}

	samples->empty = false;
	input.close();
	return samples;
}

/*----------------------------------------------------------*
 * Load data from .txt file (Embrapa dataset)		        *
 *----------------------------------------------------------*/

Sample* Data::load_txt(string file){
	ifstream input(file.c_str());
	string str, item;
	int n, dim, n1, d, size;
	Sample *sam_temp = new Sample;

	if(!input){
        cout << "File could not be opened!" << endl;
    	return NULL;
   	}

	n1 = d = size = 0;

	//error check
	while(getline(input, str)){
		istringstream ss(str);
		n1 = 0;
		dim = 0;

		while(getline(ss, item, ' ')){
			if(n1 >= 2){
				dim++;
			}
			n1++;
		}

		if(size != 0 &&  dim != d ){
				cerr << dim << " " << samples->dim << endl;
				cerr << "All the samples must have the same dimension!" << endl;
				delete sam_temp;
				return NULL;
		}

		d = dim;
		size++;
	}

	//Initialize size and dim
	sam_temp->size = size;
	sam_temp->dim = dim;

	//Set features names
	sam_temp->fnames.assign(dim, 0);
	iota(sam_temp->fnames.begin(), sam_temp->fnames.end(), 1);

	input.clear();
	input.seekg(0, ios::beg);

	//Allocate memory for points array
	sam_temp->points.assign(size, NULL);

	size = 0;

	//get line from file (sample)
	while(getline(input, str)){
		Point *new_point = new Point;
		istringstream ss(str);
		n = 0;

		ss.clear();
		ss.str(str);

		//Allocate memory for features
		new_point->x.assign(dim, 0.0);

		//read features from line
		while(getline(ss, item, ' ')){
			if(n >= 2){
				if(is_number(item))
					new_point->x[n - 2] = stoin(item);
				else{ clog << "Warning: point[" << size  << "] " << n-2 << " feature is not a number." << endl; }
				new_point->y = 0;
			}
			n++;
		}

		sam_temp->points[size++] = new_point;
		new_point->id = size;
	}

	sam_temp->empty = false;
	input.close();

	return sam_temp;
}

/*----------------------------------------------------------*
 * Save support vectors and alpha                           *
 *----------------------------------------------------------*/

int Data::write(string fname, Sample *sample, int sv_only){
	int ret = 0;

	string out_file_temp = "DB/out/";
	out_file_temp = out_file_temp + fname;

	srand(time(NULL));
	unsigned int timestamp = rand()%(100000 + 1) * rand()%(100000 + 1);
	string stimestamp;
	srand(0);

	cout << "Timestamp: " << timestamp << "\n\n";

	int i, j, k;
	unsigned int digit;
	string temp;

	for(i = 0; timestamp != 0; ++i){
		digit = timestamp % 10;
		temp.push_back((char)(digit + 48));
		timestamp /= 10;
	}

	for(j = i-1, k = 0; j >= 0; j--, k++)
		stimestamp.push_back(temp[j]);

	out_file_temp = out_file_temp + "_";
	out_file_temp = out_file_temp + stimestamp;
	out_file_temp = out_file_temp + ".data";

	ofstream outfile(out_file_temp.c_str());

	if(!outfile){
		cerr << "The file could not be opened!\n" << endl;
		return 0;
	}

	ret = write_routine(outfile, sample, sv_only);
	if(outfile.is_open()) outfile.close();

	return ret;
}

/*----------------------------------------------------------*
 * Save weight vector                                       *
 *----------------------------------------------------------*/

int Data::write_weights(string fname, double *w, int dim){
	ofstream outfile(fname.c_str());
	int i;

	if(!outfile){
		cerr << "The file could not be opened!\n" << endl;
		return 0;
	}

	outfile << dim << endl;
	for(i = 0; i < dim; ++i)
		outfile << w[i] << " ";
	outfile << endl;

	outfile.close();
	return 1;
}

/*----------------------------------------------------------*
 * load weight vector                                       *
 *----------------------------------------------------------*/

int Data::load_weights(string fname, double **w, int dim){
	int i, d;
	double val;
	string line;
	ifstream infile(fname.c_str());

	if(!infile){
		cerr << "The file could not be opened!\n" << endl;
		return 0;
	}

	infile >> d;
	if(d != dim) return 0;

	(*w) = new double[dim];
	if(!(*w)) return 0;

	while(getline(infile, line)){
  		istringstream iss(line);
  		i = 0;
		while(iss >> val){
			(*w)[i] = val;
			i++;
		}
	}
	infile.close();

	return 1;
}

/*----------------------------------------------------------*
 * Save support vectors and alpha                           *
 *----------------------------------------------------------*/

int Data::write_routine(ofstream& outfile, Sample *sample, int sv_only){
	int i = 0, j = 0;

	if(!outfile){
		cerr << "The file could not be opened!\n" << endl;
		return 0;
	}

	for(i = 0; i < sample->size; ++i){
		if((sv_only && sample->points[i]->alpha > 0) || !sv_only){
			outfile << sample->points[i]->y << " ";
			for(j = 0; j < sample->dim; ++j)
				outfile << sample->fnames[j] << ":" << sample->points[i]->x[j] << " ";
			outfile << endl;
		}
	}

	return 1;
}

int Data::generate_classes(int option){
	return Embrapa::generate_classes(samples, option);
}

/*----------------------------------------------------------*
 * Returns data array with only features in array           *
 *----------------------------------------------------------*/

Sample* Data::insert_features(Sample *samples, vector<int> ins_feat, int fsize, int verbose){
	int i = 0, j = 0;
	int s		= 0;
	int dim		= samples->dim;
	int size	= samples->size;
	int saveflag = 0;
	int offset 	= 0;
	Sample *smout = NULL;

	//Sorting data
	sort(ins_feat.begin(), ins_feat.end());

	if(verbose > 1){
		for(i = 0; i < fsize; ++i, cout << "--> " << ins_feat[i])
			cout << "---------------------------------------------\n";
	}

	//error check
	if(fsize > dim){ cerr << "Error: InsertFeature, fsize(" << ins_feat.size() << ")>dim(" << dim << ")\n"; return NULL; }

	//Allocating space for new sample
	smout = new Sample;
	if(smout == NULL){ cerr << "Error: Out of memory 30\n"; exit(1); }

	//Allocating space for new array
	smout->size = size;

	if(samples->index){
		smout->index = new int[size];
		if(smout->index == NULL){ cerr << "Error: Out of memory 32\n"; exit(1); }
		smout->index = samples->index;
	}else smout->index = NULL;

	//copying data
	smout->bias = samples->bias;
	smout->dim	= samples->dim;

	//Copying norms p and q, max_time and mult_tempo
	smout->q		  = samples->q;
	smout->p 		  = samples->p;
	smout->max_time   = samples->max_time;
	smout->mult_tempo = samples->mult_tempo;

	//Copying kernel params
	smout->kernel_type  = samples->kernel_type;
	smout->kernel_param = samples->kernel_param;

	//Copying flexibility, norm, margin, alpha prox. and normalized
	smout->flexible    = samples->flexible;
	smout->norm 	   = samples->norm;
	smout->margin 	   = samples->margin;
	smout->alpha_aprox = samples->alpha_aprox;
	smout->normalized  = samples->normalized;

	//copying size of comite
	smout->timesPerceptron = samples->timesPerceptron;
	smout->permutation	   = samples->permutation;
	smout->comite 		   = samples->comite;
	smout->similarity 	   = samples->similarity;

    smout->points.swap(samples->points);
    vector<int> fnames;
    fnames.resize(fsize, 0);

    //Copying information to new data array
    for(i = 0; i < size; ++i){
		//Allocating space for vector x
		vector<double> x(fsize);

		//Copying features
		s = 0; offset = 0;
		for(j = 0; j < dim; ++j){
			saveflag = 0;
			if(offset < fsize && samples->fnames[j] == ins_feat[offset]){
				saveflag = 1;
				offset++;
			}

			if(saveflag){
				x[s] = smout->points[i]->x[j];
				fnames[s] = samples->fnames[j];
				s++;
			}
		}
		smout->points[i]->x.swap(x);

		//error check
		if(s != fsize){
			cout << "Error: Something went wrong on InsertFeature\n";
			cout << "s = " << s << ", dim = " << dim << ", fsize = " << fsize << endl;
			exit(1);
		}
	}
	smout->fnames.swap(fnames);

	//setting up dimension
	smout->dim = fsize;

	return smout;
}

Sample* Data::join_samples(Sample *sample1, Sample *sample2){
	int i = 0, j  = 0;
	int index 	  = 0;
	int s 		  = 0;
	int dim 	  = sample1->dim;
	Sample *smout = NULL;

	if(sample1->dim > sample2->dim){
		cerr << "Error: sample1 dimension must be less or equal to sample2\n";
		exit(1);
	}

	//Allocating space for new sample
	smout = new Sample;
	if(smout == NULL){ cerr << "Error: Out of memory 1\n"; exit(1); }

	//Allocating space for new array
	smout->size	= sample1->size + sample2->size;

	if(sample1->index && sample2->index){
		smout->index = new int[smout->size];
		if(smout->index == NULL){ cerr << "Error: Out of memory 3\n"; exit(1); }
		for(i = 0; i < sample1->size; ++i) smout->index[i] = sample1->index[i];
		for(i = 0; i < sample2->size; ++i) smout->index[i+sample1->size] = sample2->index[i+sample1->size];
	}else smout->index = NULL;

	//Allocating space for new fnames
	smout->fnames.resize(dim, 0);

	//copying fnames
	for(j = 0; j < dim; ++j) smout->fnames[j] = sample1->fnames[j];

	//copying bias
	smout->bias = sample1->bias;
	smout->dim	= sample1->dim;

	//Copying norms p and q, max_time and mult_tempo
	smout->q		  = sample1->q;
	smout->p 		  = sample1->p;
	smout->max_time   = sample1->max_time;
	smout->mult_tempo = sample1->mult_tempo;

	//Copying kernel params
	smout->kernel_type  = sample1->kernel_type;
	smout->kernel_param = sample1->kernel_param;

	//Copying flexibility, norm, margin, alpha prox. and normalized
	smout->flexible    = sample1->flexible;
	smout->norm 	   = sample1->norm;
	smout->margin 	   = sample1->margin;
	smout->alpha_aprox = sample1->alpha_aprox;
	smout->normalized  = sample1->normalized;

	//copying size of comite
	smout->timesPerceptron = sample1->timesPerceptron;
	smout->permutation	   = sample1->permutation;
	smout->comite 		   = sample1->comite;
	smout->similarity 	   = sample1->similarity;

	smout->points.resize(smout->size);
	for(i = 0; i < smout->size; ++i) smout->points[i] = new Point;

	//copying sample1 information to new data array
	for(i = 0; i < sample1->size; ++i){
		//initializing
		smout->points[i]->y = sample1->points[i]->y;
		smout->points[i]->alpha = sample1->points[i]->alpha;

		//Allocating space for vector x
		smout->points[i]->x.resize(dim, 0.0);

		//copying features
		for(j = 0; j < dim; ++j) smout->points[i]->x[j] = sample1->points[i]->x[j];
	}

	//copying sample2 information to new data array
	for(i = 0; i < sample2->size; ++i){
		index = i + sample1->size;
		smout->points[index]->y = sample2->points[i]->y;
		smout->points[index]->alpha = sample2->points[i]->alpha;

		smout->points[index]->x.resize(dim);

		//copying features
		s = 0;
		for(j = 0; j < sample2->dim; ++j){
			if(sample1->fnames[s] == sample2->fnames[j]){
				smout->points[index]->x[s] = sample2->points[i]->x[j];
				s++;
			}
		}
	}

	if(s != dim){ cerr << "Error: s and dim dont match, (" << s << ", " << dim << ")\n"; exit(0); }

	return smout;
}

Sample* Data::remove_features(Sample *samples, vector<int> rem_feat){
	int i = 0, removed = 0, psize = samples->points.size(), rsize = rem_feat.size();
	vector<Point*>::iterator itr = samples->points.begin();

	//Error check
	if(samples->fnames.size() == 1){ cerr << "Error: RemoveFeature, only one feature left.\n"; return NULL; }
	if(rem_feat.size() >= samples->fnames.size()){ cerr << "Error: RemoveFeature, more features to remove than exist.\n";	
	 return NULL;}

	sort(rem_feat.begin(),rem_feat.end());

	//Set new dimension
	samples->fnames.resize(samples->dim);
	
	//Remove features from each point
	for(i = 0; i < rsize; ++i){
		int f = rem_feat[i];
		if(f == 0) continue;
		for(i = 0; i < psize; ++i){
			samples->points[i]->x.erase(samples->points[i]->x.begin() + (f - removed - 1));
		}
		removed++;
	}
	
	samples->dim = samples->dim - removed;
	
	//Fill new fnames
	iota(samples->fnames.begin(), samples->fnames.end(), 1);

    return samples;
}

/*----------------------------------------------------------*
 * Erase points without ID                                 *
 *----------------------------------------------------------*/

Sample* Data::remove_points_without_id(Sample *samples, vector<int> ids){
	vector<Point*>::iterator p = samples->points.begin();
	bool save = false;
	int i = 0, idsize = ids.size();
	
	for(; p != samples->points.end();){
		save = true;
		Point* po = *(p);
		for(i = 0; i < idsize; ++i){
			if(po->id == ids[i]){
				save = false;
				break;
			}
		}
		if(save) p++;
		else{
			Point *po = (*p);
			delete po;
			p = samples->points.erase(p);
			samples->size--;
		}
	}

	return samples;
}

/*----------------------------------------------------------*
 * add a point into a sample                                *
 *----------------------------------------------------------*/

Sample* Data::insert_point(Sample* samp, Sample* samp_in, int index){
	int i = 0, j  = 0;
	int dim 	  = samp->dim;
	Sample *smout = NULL;

	//Allocating space for new array
	samp->size = samp->size + 1;
	samp->points.resize(samp->size, new Point);

	//Allocating space for index array (if exists)
	if(samp->index){
		int* ind = samp->index;
		samp->index = NULL;
		samp->index = new int[samp->size];
		if(smout->index == NULL){ cerr << "Error: Out of memory 16\n"; exit(1); }
		for(i = 0; i < samp->size; ++i) samp->index[i] = ind[i];
		samp->index[i] = index;
		delete [] ind;
	}else samp->index = NULL;

	//insert a new point into new data array
	i = samp->points.size() - 1;
	samp->points[i]->y = samp_in->points[index]->y;
	samp->points[i]->alpha = samp_in->points[index]->alpha;

	if(samp->index) samp->index[i] = i;

	//Allocating space for vector x
	samp->points[i]->x.resize(dim);

	//Copying features
	for(j = 0; j < dim; ++j)
		samp->points[i]->x[j] = samp_in->points[index]->x[j];

	return samp;
}

/*----------------------------------------------------------*
 * return sample with one less data point                     *
 *----------------------------------------------------------*/

Sample* Data::remove_point(Sample *samp, int ind){
	int size = samp->size;
	int i;

	if(size == 1){ cout << "Error: RemovePoint, only one point left\n"; return NULL; }

	if(ind > size-1 || ind < 0) return samp;

	samp->size = size - 1;

	if(samp->index){
		int *index = new int[size];
		if(!index){ cout << "Error: Out of memory 23\n"; exit(1); }
	
		for(i = 0; i < size; ++i){
			if(i >= ind){
				index[i-1] = samp->index[i] - 1;
			}else index[i] = samp->index[i];
		}
	}else samp->index = NULL;
	
	if(samp->npos_samples > 0 || samp->nneg_samples > 0){
		if(samp->points[ind]->y == 1) samp->npos_samples--;
		else if(samp->points[ind]->y == -1) samp->nneg_samples--;
	}
	
	vector<Point*>::iterator p = samp->points.begin() + ind;
	Point *po = (*p);
	delete po;
	samp->points.erase(samp->points.begin() + ind);

	return samp;
}

/*----------------------------------------------------------*
 * Divide sample into train and test                        *
 *----------------------------------------------------------*/

Sample* Data::part_train_test(Sample* train_sample, Sample* test_sample, int fold, unsigned int seed, int verbose){
	int i = 0, j = 0;
	int qtdpos = 0, qtdneg = 0;
	Sample *sample_pos = NULL, *sample_neg = NULL;
	
	sample_pos = Sample::copy_sample_zero(train_sample);
	sample_neg = Sample::copy_sample_zero(train_sample);

	for(i = 0; i < train_sample->size; ++i){
		if(train_sample->points[i]->y == 1)
			sample_pos = insert_point(sample_pos, train_sample, i);
		else
			sample_neg = insert_point(sample_neg, train_sample, i);
	}

	qtdpos = sample_pos->size;
	qtdneg = sample_neg->size;

	if(verbose){
		cout << "\nTotal of points: " << train_sample->size << endl;
		cout << "Number of pos.: " << qtdpos << endl;
		cout << "Number of neg.: " << qtdneg << endl;
	}

    srand(seed);

    for(i = 0; i < sample_pos->size; i++){
        Point* aux;
        j = rand()%(sample_pos->size);
        aux = sample_pos->points[i];
        sample_pos->points[i] = sample_pos->points[j];
        sample_pos->points[j] = aux;
    }
    for(i = 0; i < sample_neg->size; i++){
        Point* aux;
        j = rand()%(sample_neg->size);
        aux = sample_neg->points[i];
        sample_neg->points[i] = sample_neg->points[j];
        sample_neg->points[j] = aux;
    }

   	delete train_sample;
	delete test_sample;

    train_sample = Sample::copy_sample_zero(sample_pos);
    test_sample  = Sample::copy_sample_zero(sample_pos);

    for(j = 0; j < sample_pos->size*(fold-1)/fold; j++)
        train_sample = insert_point(train_sample, sample_pos, j);
    for(; j < sample_pos->size; j++)
        test_sample = insert_point(test_sample, sample_pos, j);

    for(j = 0; j < sample_neg->size/fold; j++)
        test_sample = insert_point(test_sample, sample_neg, j);
    for(; j < sample_neg->size; j++)
        train_sample = insert_point(train_sample, sample_neg, j);

	train_sample->fnames.assign(train_sample->dim, 0);
	test_sample->fnames.assign(test_sample->dim, 0);
	for(i = 1; i <= train_sample->dim; i++){
		train_sample->fnames[i-1] = i;
		test_sample->fnames[i-1] = i;
	}

    if(verbose){
        cout << "Train points: " << train_sample->size <<endl;
        cout << "Test points: " << test_sample->size << "\n" <<endl;
    }

	delete sample_pos;
	delete sample_neg;
	
	return test_sample;
}

void Data::remove_test_sample_features(Sample *train_sample, Sample *test_sample, int verbose){
	int i, j, k;
	int totalfeat = test_sample->dim - train_sample->dim;

	if(test_sample->dim > train_sample->dim){
		vector<int> feats(totalfeat, 0);

		for(k = 0, j = 0, i = 0; i < test_sample->dim; ++i){
			if(test_sample->fnames[i] != train_sample->fnames[j])
				feats[k++] = test_sample->fnames[i];
			else if(j < train_sample->dim-1)
				j++;
		}

		if(totalfeat != k)
			cerr << "Error in remotion: It was to remove " << totalfeat << " but were removed " << k << ".\n";
		Sample *sample_temp = remove_features(test_sample, feats);
		delete test_sample;
		test_sample = sample_temp;
		sample_temp = NULL;

		if(verbose){
			cout << "Remotion:\n";
			for(i = 0; i < k; ++i){
				cout << feats[i] << " ";
				if((i + 1) % 20 == 0) cout << endl;
			}
			cout << endl;
		}
	}
}

/*----------------------------------------------------------*
 * Computes norm in dual variables                          *
 *----------------------------------------------------------*/

double Data::calculate_norm(double **K){
	register int i, j;
	register double sum, sum1;
	vector<Point*> points = samples->points;

	sum = sum1 = 0;

	for(i = 0; i < samples->size; ++i){
		for(j = 0; j < samples->size; j++){
			sum1 += points[j]->alpha * points[j]->y * K[i][j];
			sum += points[i]->y * points[i]->alpha * sum1;
		}
	}

	return sqrt(sum);
}

/*----------------------------------------------------------*
 * Returns variance of the data.                            *
 *----------------------------------------------------------*/

double Data::get_variance(int index){
	register int i, j;
	register double norm = 0.0;
	register double sum = 0.0;
	double *avg = new double[samples->dim];

	for(j = 0; j < samples->dim; ++j){
		if(index < 0 || samples->fnames[j] != index){
			avg[j] = 0.0;

			for(i = 0; i < samples->size; ++i){
				avg[j] += samples->points[i]->x[j];
			}
			avg[j] = avg[j] / samples->size;
		}
	}

	for(i = 0; i < samples->size; ++i){
		for(j = 0; j < samples->dim; ++j){
			if(index < 0 || samples->fnames[j] != index){
				norm += pow(avg[j] - samples->points[i]->x[j], 2);
			}
			sum += norm;
		}
	}

	sum = sum/samples->size;

	delete [] avg;

	return sum;
}

/*----------------------------------------------------------*
 * Returns average of a feature.                            *
 *----------------------------------------------------------*/

double Data::get_feature_average(int index){
    int i;
    double sum = 0.0;

	for(i = 0; i < samples->size; ++i){
		sum += samples->points[i]->x[index];
	}
	sum /= samples->size;
	
	return sum;
}

/*----------------------------------------------------------*
 * change the x vector of a sample                          *
 *----------------------------------------------------------*/

void Data::change_x_vector(Sample *samp, int *index){
	int i, j;
	int size = samp->size;
	int dim = samp->dim;

	vector<vector<double> > new_x(size, vector<double>(dim, 0.0));
	vector<int> new_y(size, 0);

	for(i = 0; i < size; ++i){
		for(j = 0; j < dim; ++j){
			new_x[i][j] = samp->points[index[i]]->x[j];
		}
		new_y[i] = samp->points[index[i]]->y;
	}

	for(i = 0; i < size; ++i){
		samp->points[i]->x.swap(new_x[i]);
		samp->points[i]->y = new_y[i];
	}
}

/*----------------------------------------------------------*
 * Returns radius of the ball that circ. the data.          *
 *----------------------------------------------------------*/

double Data::get_radius(int index, double q){
	register int i = 0, j = 0;
	register double norm = 0.0;
	double max = 1.0;
	double *avg = NULL;

	if(q == 2){
		avg = new double[samples->dim];

		for(j = 0; j < samples->dim; ++j){
			if(index < 0 || samples->fnames[j] != index){
				avg[j] = 0.0;

				for(i = 0; i < samples->size; ++i){
					avg[j] += samples->points[i]->x[j];
				}
				avg[j] = avg[j] / samples->size;
			}
		}

		for(max = 0, i = 0; i < samples->size; ++i){
			for(norm = 0, j = 0; j < samples->dim; ++j){
				if(index < 0 || samples->fnames[j] != index){
					norm += pow(avg[j] - samples->points[i]->x[j], 2);
				}

				norm = sqrt(norm);

				if(max < norm) max = norm;
			}
		}

		delete [] avg;
	}else if(q == 1){
		for(max = 0, i = 0; i < samples->size; ++i){
			for(j = 0; j < samples->dim; ++j){
				if(index < 0 || samples->fnames[j] != index)
					if(max < fabs(samples->points[i]->x[j]))
						max = fabs(samples->points[i]->x[j]);
			}
		}
	}

	return max;
}

/*----------------------------------------------------------*
 * Returns distance of centers of classes                   *
 *----------------------------------------------------------*/

double Data::get_dist_centers(int index){
 	register int i = 0, j = 0;
 	double *avg_pos = NULL;
 	double *avg_neg = NULL;
 	register double dist = 0.0;
 	register int size_pos = 0, size_neg = 0;

 	for(size_pos = 0, size_neg = 0, i = 0; i < samples->size; ++i){
 		if(samples->points[i]->y == 1)	size_pos++;
 		else 							size_neg++;
 	}

 	avg_pos = new double[samples->dim];
 	avg_neg = new double[samples->dim];

 	for(j = 0; j < samples->dim; ++j){
 		avg_pos[j] = 0.0;
 		avg_neg[j] = 0.0;

 		for(i = 0; i < samples->size; ++i){
 			if(samples->points[i]->y == 1){
 				avg_pos[j] += samples->points[i]->x[j];
 			}else
 				avg_neg[j] += samples->points[i]->x[j];
 		}

 		avg_pos[j] /= (double)size_pos;
 		avg_neg[j] /= (double)size_neg;
 	}

 	for(dist = 0.0, j = 0; j < samples->dim; ++j){
 		if(index < 0 || samples->fnames[j] != index)
 			dist += pow(avg_pos[j] - avg_neg[j], 2);
 	}

 	delete [] avg_pos;
 	delete [] avg_neg;

 	return sqrt(dist);
}

/*----------------------------------------------------------*
 * Returns sample with dataset normalized                  *
 *----------------------------------------------------------*/

void Data::normalize_dataset(){
	int i = 0, j = 0;
	Sample *samp = samples;
	int dim  	  = samp->dim;
	int size	  = samp->size;
	Sample *smout = NULL;
	double norm;

	//Allocating space for new sample
	smout = new Sample;
	if(smout == NULL){ cerr << "Error: Out of memory\n"; exit(1); }

	//copying data
	smout->size         = samp->size;
    smout->dim          = samp->dim+1;
    smout->bias         = samp->bias;
    smout->p            = samp->p;
    smout->q            = samp->q;
    smout->kernel_type  = samp->kernel_type;
    smout->kernel_param = samp->kernel_param;
    smout->max_time     = samp->max_time;
    smout->mult_tempo   = samp->mult_tempo;
    smout->alpha_aprox  = samp->alpha_aprox;
    smout->flexible     = samp->flexible;
    smout->norm         = samp->norm;
    smout->margin       = samp->margin;
    smout->normalized   = 1;
    smout->timesPerceptron  = samp->timesPerceptron;
    smout->permutation  = samp->permutation;
    smout->comite = samp->comite;
    smout->empty = samp->empty;
    smout->similarity = samp->similarity;

	//Allocating space for new array
	smout->points.resize(size);

	//Allocating space for index array (if exists)
	if(samp->index){
		smout->index = new int[size];
		if(smout->index == NULL){ cout << "Error: Out of memory\n"; exit(1); }
		for(i = 0; i < size; ++i) smout->index[i] = samp->index[i];
	}else smout->index = NULL;

	//Allocating space for new fnames
	smout->fnames.resize(dim + 1, 0);

	//Copying information to new data array
	for(i = 0; i < size; ++i){
		//initializing
		smout->points[i] = new Point;
		smout->points[i]->y = samp->points[i]->y;
		smout->points[i]->alpha = samp->points[i]->alpha;

		//Allocating space for vector x
		smout->points[i]->x.resize(dim + 1, 0.0);

		//Copying features
		for(norm = 0, j = 0; j < dim; ++j){
			smout->points[i]->x[j] = samp->points[i]->x[j];
			smout->fnames[j]	   = samp->fnames[j];
			norm += pow(fabs(smout->points[i]->x[j]), smout->p);
		}
		smout->points[i]->x[j] = 1;
		smout->fnames[j] = j + 1;
		norm += pow(fabs(smout->points[i]->x[j]), smout->p);
		norm = pow(norm, 1.0/smout->p);
		for(j = 0; j < dim + 1; ++j)
			smout->points[i]->x[j] /= norm;
	}

	//delete samp;
	//samp = NULL;
	delete samples;
	samples = smout;
}

/*----------------------------------------------------------*
 * Returns data normalized                                  *
 *----------------------------------------------------------*/

void Data::normalized(double *w, double q, int dim){
	register int i = 0;
	register double norm = 0.0;

	for(i = 0; i < dim; ++i)
		norm += pow(fabs(w[i]), q);

	norm = pow(norm, 1.0/q);

	for(i = 0; i < dim; ++i)
		w[i] /= norm;
}

/*----------------------------------------------------------*
 * Returns data normalized to max x norm                    *
 *----------------------------------------------------------*/

void Data::norm(double p){
	register int i = 0, j = 0;
	register double norm = 0.0;

	for(i = 0; i < samples->size; ++i){
		for(norm = 0, j = 0; j < samples->dim; ++j){
			norm += pow(fabs(samples->points[i]->x[j]),p);
		}
		norm = pow(norm, 1.0/p);
		for(j = 0; j < samples->dim; ++j){
			samples->points[i]->x[j] /= norm;
		}
	}
}

/*----------------------------------------------------------*
 * Returns distance of centers of classes without feats     *
 *----------------------------------------------------------*/

double Data::get_dist_centers_without_feats(vector<int> feats, int index){
	register int i =0, j = 0;
	double *avg_pos = NULL;
	double *avg_neg = NULL;
	register double dist = 0.0;
	register int size_pos = 0, size_neg = 0, featsize = feats.size();

	for(size_pos = 0, size_neg = 0, i = 0; i < samples->size; ++i){
		if(samples->points[i]->y == 1)	size_pos++;
		else							size_neg++;
	}

	avg_pos = new double[samples->dim];
	avg_neg = new double[samples->dim];

	for(j = 0; j < samples->dim; ++j){
		avg_pos[j] = 0.0;
		avg_neg[j] = 0.0;

		for(i = 0; i < samples->size; ++i){
			if(samples->points[i]->y == 1)
				avg_pos[j] += samples->points[i]->x[j];
			else
				avg_neg[j] += samples->points[i]->x[j];
		}

		avg_pos[j] /= (double) size_pos;
		avg_neg[j] /= (double) size_neg;
	}

	for(dist = 0.0, j = 0; j < samples->dim; ++j){
		for(i = 0; i < featsize; ++i){
			if(samples->fnames[j] == feats[i])
				dist -= pow(avg_pos[j] - avg_neg[j], 2);
		}
	}


	return sqrt(dist);
}

/*----------------------------------------------------------*
 * Returns standard deviation of a vector.                  *
 *----------------------------------------------------------*/

double Data::standart_deviation(vector<double> vet){
	if(vet.size() == 1) return 0.0;
	int i;
	double avg, sd, vetsize = vet.size();

	for(avg = 0.0, i = 0; i < vetsize; ++i){
		avg += vet[i];
	}

	avg /= vet.size();

	for(sd = 0.0, i = 0; i < vetsize; ++i){
		sd = (vet[i] - avg)*(vet[i] - avg);
	}

	return sqrt(sd/(vet.size() - 1));
}

/*----------------------------------------------------------*
 * Returns standard deviation of a feature.                  *
 *----------------------------------------------------------*/

double Data::get_feature_std(int index){
	if(samples->size == 1) return 0.0;
	int i;
	double avg, sd, vetsize = samples->size;

	for(avg = 0.0, i = 0; i < vetsize; ++i){
		avg += samples->points[i]->x[index];
	}

	avg /= vetsize;

	for(sd = 0.0, i = 0; i < vetsize; ++i){
		sd = (samples->points[i]->x[index] - avg)*(samples->points[i]->x[index] - avg);
	}

	return sqrt(sd/(vetsize - 1));
}

Sample* Data::get_samples(){
	return this->samples;
}

void Data::set_samples(Sample* samples){
	delete this->samples;
	this->samples = NULL;
	this->samples = samples;
	samples->empty = false;
	if(this->samples == NULL){ cout << "Error: Out of memory!\n" << endl; }
}

void Data::set_norm(double p, double q){
	samples->p = p;
	samples->q = q;
}

void Data::set_norm(double p){
	samples->p = p;
}

bool Data::is_normalized(){
	return samples->normalized;
}

vector<int> Data::get_fnames(){
	return samples->fnames;
}

int Data::get_size(){
	return samples->size;
}

int Data::get_dim(){
	return samples->dim;
}

void Data::set_maxtime(double val){
	samples->max_time = val;
}

void Data::print(){
	for(Point *p : samples->points){
		if(!p->x.empty()){
			cout << p->y << " ";
			for(double x : p->x){
				cout << x << " ";
			}
			cout << endl;
		}
	}
}

vector<Point*> Data::get_points(){
	return samples->points;
}

void Data::clear(){
	delete samples;
	samples = NULL;
}

bool Data::empty(){
	return (samples == NULL)?true:samples->empty;
}

