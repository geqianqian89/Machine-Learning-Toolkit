#include <cmath>

#include "../includes/Kernel.hpp"

using namespace std;

Kernel::Kernel(int type, double param){
  this->type = type;
  this->param = param;
}

Kernel::Kernel(dMatrix kernel_matrix){
    this->K = kernel_matrix;
}

int Kernel::getType(){
    return type;
}

double Kernel::getParam(){
    return param;
}

void Kernel::setType(int type){
    this->type = type;
}

void Kernel::setParam(int param){
    if(type > 0){
        this->param = param;
    }
}

void Kernel::setKernelMatrix(dMatrix K){
    this->K = K;
}

dMatrix Kernel::getKernelMatrix(){
    return K;
}

void Kernel::compute(Data samples){
    int i, j, size = samples.getSize(), dim = samples.getDim();
    vector<Point> points = samples.getPoints();

    K.assign(size, vector<double>(size, 0.0));

    //Calculating Matrix
    for(i = 0; i < size; ++i){
        for(j = i; j < size; ++j){
            K[i][j] = function(points[i], points[j], dim);
            K[j][i] = K[i][j];
        }
    }
}

double Kernel::function(Point one, Point two, int dim){
    int i = 0;
    double t, sum = 0.0;
    vector<double> a = one.x, b = two.x;

   // a.erase(a.end());
    //b.erase(b.end());

    switch(type)
    {
        case 0: //Produto Interno
            for(i = 0; i < dim; ++i)
                sum += a[i] * b[i];
            break;
        case 1: //Polinomial
            for(i = 0; i < dim; ++i)
                sum += a[i] * b[i];
        //    sum = (param > 1) ? pow(sum+1, param) : sum;
            sum = (param > 1) ? pow(sum, param) : sum;
            break;

        case 2: //Gaussiano
            for(i = 0; i < dim; ++i)
            { t = a[i] - b[i]; sum += t * t; }
            sum = exp(-1 * sum * param);
            break;
    }

    /*The '+1' here accounts for the bias term "b" in SVM formulation since
      <w,x> = \sum_i \alpha_i y_i k(x_i,x) + b and b=\sum_i \alpha_i y_i*/

    return sum;// + 1.0f;
}

double Kernel::norm(Data data){
    int i, j, size = data.getSize();
    double sum, sum1;
    vector<Point> points = data.getPoints();

    sum = sum1 = 0;

    for(i = 0; i < size; ++i){
        for(j = 0; j < size; j++){
            sum1 += points[j].alpha * points[j].y * K[i][j];
            sum += points[i].y * points[i].alpha * sum1;
        }
    }

    return sqrt(sum);
}
