/*****************************************************
 * Kernel functions lib                              *
 *****************************************************/
 
#ifndef KERNEL_H_INCLUDED
#define KERNEL_H_INCLUDED

double** kernel_generate_matrix(Sample *sample);
double** kernel_generate_matrix_H(Sample *sample);
double** kernel_generate_matrix_H_without_dim(Sample *sample, int dim);
double kernel_function(Point *one, Point *two, int dim);
double kernel_function_without_dim(Point *one, Point *two, int j, int dim);
double kernel_feature_space_norm(Sample *sample, double** matrix);
double kernel_feature_space_norm_with_bias(Sample *sample, double** matrix);
double kernel_norm_between_two_alphas(Sample *sample, double* alpha2, double** matrix);
double** kernel_read_matrix(string fname);
double** kernel_remove_element(double** o_matrix, int size, int num);
void kernel_print_matrix(double** matrix, int size);
void kernel_free_matrix(double** matrix, int size);

#endif
