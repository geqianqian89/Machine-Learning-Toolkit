#ifndef PERCEPTRON_H_INCLUDED
#define PERCEPTRON_H_INCLUDED

bool perceptron_primal_norm2(train_data *data, int& steps, int& ctot, int *index, double q, double start_time, double max_time, double rate); 
bool perceptron_fixed_margin_primal(train_data *data, double gamma, int* steps, int* ctot, int *index, double q, double start_time, double max_time, double rate);
int perceptron_fixed_margin_dual(datadual *data, double gamma, int& steps, int& ctot, int *index, double start_time, double max_time, double rate);
int perceptron_dual(datadual *data, int& steps, int& ctot, int *index, double start_time, double max_time, double rate);

#endif // PERCEPTRON_H_INCLUDED
