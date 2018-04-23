/*********************************************************************************
 * kernel.cpp: Kernel functions lib                                                *
 *********************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <fstream>
#include "data.h"
#include "kernel.h"

using namespace std;

double kernel_param;
int kernel_type;

/*----------------------------------------------------------*
 * Generates Kernel matrix                                  *
 *----------------------------------------------------------*/

double** kernel_generate_matrix(Sample *sample){
	double **matrix = NULL;
	int i = 0, j = 0, size = sample->size;
	
	//Allocating space for new matrix
	matrix = new double*[size];
	if(!matrix){ cerr << "Error: Out of memory\n"; exit(1); }
	
	for(i = 0; i < size; ++i){
		matrix[i] = new double[size];
		if(!matrix[i]){ cerr << "Error: Out of memory\n"; exit(1); }
	}
	
	//Calculating Matrix
	for(i = 0; i < size; ++i){
		for(j = i; j < size; ++j){
			matrix[i][j] = kernel_function(sample->points[i], sample->points[j], sample->dim);
            matrix[j][i] = matrix[i][j];
		}
	}
	
	return matrix;
}

/*----------------------------------------------------------*
 * Generates H matrix                                       *
 *----------------------------------------------------------*/

double** kernel_generate_matrix_H(Sample *sample){
	double **matrix = NULL;
	int i = 0, j = 0, size = sample->size;
	
	//Allocating space for new matrix
	matrix = new double*[size];
	if(!matrix){ cerr << "Error: Out of memory\n"; exit(1); }

	for(i = 0; i < size; ++i){
		matrix[i] = new double[size];
		if(!matrix[i]){ cerr << "Error: Out of memory\n"; exit(1); }
	}
	
	//Calculating Matrix
	for(i = 0; i < size; ++i){
		for(j = 0; j < size; ++j){
			matrix[i][j] = kernel_function(sample->points[i], sample->points[j], sample->dim) * sample->points[i]->y * sample->points[j]->y;
			matrix[j][i] = matrix[i][j];
		}
	}
	
	return matrix;
}

/*----------------------------------------------------------*
 * Generates H matrix without a dimenson                    *
 *----------------------------------------------------------*/

double** kernel_generate_matrix_H_without_dim(Sample *sample, int dim){
	double **matrix = NULL;
	int i = 0, j = 0, size = sample->size;
	
	//Allocating space for new matrix
	matrix = new double*[size];
	if(!matrix){ cerr << "Error: Out of memory\n"; exit(1); }

	for(i = 0; i < size; ++i){
		matrix[i] = new double[size];
		if(!matrix[i]){ cerr << "Error: Out of memory\n"; exit(1); }
	}
	
	//Calculating Matrix
	for(i = 0; i < size; ++i){
		for(j = i; j < size; ++j){
			matrix[i][j] = kernel_function_without_dim(sample->points[i], sample->points[j], dim, sample->dim) * sample->points[i]->y * sample->points[j]->y;
			matrix[j][i] = matrix[i][j];
		} 
	}
	
	return matrix;
}

/*----------------------------------------------------------*
 * Calculates kernel function between two points            *
 *----------------------------------------------------------*/
 
double kernel_function(Point *one, Point *two, int dim){
	int i = 0;
	double t, sum = 0.0;
	vector<double> a = one->x, b = two->x;
	
	switch(kernel_type){
		case 0:	//Inner product
			for(i = 0; i < dim; ++i)
				sum += a[i] * b[i];	
			break;
		case 1:	//Polynomial
			for(i = 0; i < dim; ++i)
				sum += a[i] * b[i];
			sum = (kernel_param > 1) ? pow(sum+1, kernel_param) : sum;
			break;
		case 2:	//Gaussian
			for(i = 0; i < dim; ++i){
				t = a[i] - b[i];
				sum += t * t;
			}
			sum = exp(-1 * sum * kernel_param);
			break;
	}

    return sum;
}

/*------------------------------------------------------------------*
 * Calculates kernel function between two points without a dimenson *
 *------------------------------------------------------------------*/

double kernel_function_without_dim(Point *one, Point *two, int j, int dim){
	int i = 0;
	double t, sum = 0.0;
	vector<double> a = one->x, b = two->x;
	
	switch(kernel_type){
		case 0: //Inner product
			for(i = 0; i < dim; ++i){
				if(i != j)
					sum += a[i] * b[i];
			}
			break;
		case 1: //Polynomial
			for(i = 0; i < dim; ++i){
				if(i != j)
					sum += a[i] * b[i];
			}
			sum = (kernel_param > 1) ? pow(sum+1, kernel_param) : sum;
			break;
		case 2: //Gaussian
			for(i = 0; i < dim; ++i){
				if(i != j){
					t = a[i] - b[i];
					sum += t * t;
				}
			}
			sum = exp(-1 * sum * kernel_param);
			break;
	}
	
	return sum;
}

/*----------------------------------------------------------*
 * Returns norm in the feature space                        *
 *----------------------------------------------------------*/
 
double kernel_feature_space_norm(Sample *sample, double** matrix){
	int i = 0, j = 0;
	double sum1, sum;
	
	sum = sum1 = 0.0;
	
	for(i = 0; i < sample->size; ++i){
		if(sample->points[i]->alpha > 0){
			for(sum1 = 0.0, j = 0; j < sample->size; ++j){
				if(sample->points[j]->alpha > 0)
					sum1 += sample->points[j]->y * sample->points[j]->alpha * matrix[j][i];
			}
			sum += sample->points[i]->alpha * sample->points[i]->y * sum1;
		}
	}
	sum = sqrt(sum);
	
	return sum;
}

/*----------------------------------------------------------*
 * Returns norm in the feature space with bias              *
 *----------------------------------------------------------*/
 
double kernel_feature_space_norm_with_bias(Sample* sample, double** matrix){
 	int i = 0, j = 0, size = sample->size;
 	double sum1 = 0.0, sum = 0.0;
 	
 	for(i = 0; i < size; ++i){
 		if(sample->points[i]->alpha > 0){
 			for(j = 0, sum1 = 0.0; j < sample->size; ++j){
 				if(sample->points[j]->alpha > 0)
 					sum1 += sample->points[j]->y * sample->points[j]->alpha * (matrix[j][i] + 1.0);
 			}
 			sum += sample->points[i]->alpha * sample->points[i]->y * sum1;
 		}
 	}
 	sum = sqrt(sum);
 	
 	return sum;
} 

/*----------------------------------------------------------*
 * Returns norm between two alpha vectors                   *
 *----------------------------------------------------------*/

double kernel_norm_between_two_alphas(Sample *sample, double* alpha2, double** matrix){
	int i = 0, j = 0, size = sample->size;
	double sum1 = 0.0, sum = 0.0;
	
	for(i = 0; i < size; ++i){
		if(sample->points[i]->alpha > 0){
			for(j = 0, sum1 = 0.0; j < size; ++j){
				if(alpha2[j] > 0){
					sum1 += sample->points[j]->y * alpha2[j] * (matrix[j][i] + 1.0);
				}
				sum += sample->points[i]->alpha * sample->points[i]->y * sum1;
			}
		}
	}
	sum = sqrt(sum);
	
	return sum;
}

/*----------------------------------------------------------*
 * reads kernel matrix from file                            *
 *----------------------------------------------------------*/

double** kernel_read_matrix(string fname){
	int i = 0, j = 0, size = 0;
	double** matrix = NULL;
	ifstream file(fname.c_str()); 	
	string item, line;
	
	if(!file){
		cerr << "The file couldn't be opened..." << endl;
		return NULL;
	}
	
	getline(file, line); 
	stringstream ss(line);
	
	while(getline(ss, item, ' ')){ size++; }
	
	file.clear();
	file.seekg(0, ios::beg);
	
	//Allocating space for new matrix
	matrix = new double*[size];
	for(i = 0; i < size; ++i){
		matrix[i] = new double[size];
		if(!matrix[i]){ cerr << "Error: Out of memory!\n"; }
	}
	
	i = j = 0;
	
	while(getline(file, line)){
		stringstream ss(line);
		j = 0;
		while(getline(ss, item, ' ')){
			if(i >= size || j >= size) break;
			if(i == j){
				matrix[i][j] = 10.0;
			}else{
				if(Data::is_number(item)){
					matrix[j][i] = Data::stodn(item);
					matrix[i][j] = Data::stodn(item);
				}
			}
			j++;
		}
		i++;
	}
	
	file.close();
	return matrix;
}

/*----------------------------------------------------------*
 * Allocates a new matrix with one less sample point.       *
 *----------------------------------------------------------*/

double** kernel_remove_element(double** o_matrix, int size, int num){
	double** matrix = NULL;
	int i = 0, j = 0, c = 0, d = 0;
	
	//Allocating space for new matrix
	matrix = new double*[size-1];
	for(i = 0; i < size-1; ++i){
		matrix[i] = new double[size-1];
		if(!matrix[i]){ cerr << "Error: Out of memory!\n"; }
	}
	
	//Copying matrix
	for(i = 0; i < size; ++i){
		if(i != num){
			d = c;
			for(j = i; j < size; ++j){
				if(j != num){
					matrix[c][d] = o_matrix[i][j];
					matrix[d][c] = o_matrix[i][j];
					d++;
				}
			}
			c++;
		}
	}
	
	return matrix;  
}

/*----------------------------------------------------------*
 * Prints matrix                                            *
 *----------------------------------------------------------*/
 
void kernel_print_matrix(double** matrix, int size){
	int i = 0, j = 0;
	
	for(i = 0; i < size; ++i){
		for(j = 0; j < size; ++j){
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

/*----------------------------------------------------------*
 * Free matrix memory                                       *
 *----------------------------------------------------------*/

void kernel_free_matrix(double** matrix, int size){
	int i;
	
	for(i = 0; i < size; ++i)
		delete matrix[i];
	
	delete matrix;
	matrix = NULL;
}
