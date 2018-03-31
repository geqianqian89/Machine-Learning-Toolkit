#include <cmath>
#include <utility>
#include "../includes/Kernel.hpp"

using namespace std;

Kernel::Kernel(int type, double param){
    this->type = type;
    this->param = param;
}

Kernel::Kernel(dMatrix kernel_matrix){
    this->K = std::move(kernel_matrix);
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

dMatrix* Kernel::getKernelMatrixPointer(){
    return &K;
}

void Kernel::compute(Data samples){
    size_t i, j, size = samples.getSize(), dim = samples.getDim();
    vector<shared_ptr<Point> > points = samples.getPoints();

    K.assign(size, vector<double>(size, 0.0));

    //Calculating Matrix
    for(i = 0; i < size; ++i){
        for(j = i; j < size; ++j){
            K[i][j] = function(points[i], points[j], dim);
            K[j][i] = K[i][j];
        }
    }
}

double Kernel::function(shared_ptr<Point> one, shared_ptr<Point> two, int dim){
    int i = 0;
    double t, sum = 0.0;
    vector<double> a = one->x, b = two->x;

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
    size_t i, j, size = data.getSize();
    double sum, sum1;
    vector<shared_ptr<Point> > points = data.getPoints();

    sum = sum1 = 0;

    for(i = 0; i < size; ++i){
        for(j = 0; j < size; j++){
            sum1 += points[j]->alpha * points[j]->y * K[i][j];
            sum += points[i]->y * points[i]->alpha * sum1;
        }
    }

    return sqrt(sum);
}

dMatrix Kernel::generateMatrixH(const std::shared_ptr<Data> samples) {
    register int i = 0, j = 0;
    size_t size = samples->getSize(), dim = samples->getDim();
    dMatrix matrix(size, vector<double>(size));

    /* Calculating Matrix */
    for(i = 0; i < size; ++i)
        for(j = i; j < size; ++j)
        {
            matrix[i][j] = function(samples->getPoint(i), samples->getPoint(j), dim) * samples->getPoint(i)->y * samples->getPoint(j)->y;
            matrix[j][i] = matrix[i][j];
        }
    return matrix;
}

dMatrix Kernel::generateMatrixHwithoutDim(const std::shared_ptr<Data> samples, int dim) {
    register int i = 0, j = 0;
    size_t size = samples->getSize();
    dMatrix matrix(size, vector<double>(size));

    /* Calculating Matrix */
    for(i = 0; i < size; ++i)
        for(j = i; j < size; ++j)
        {
            matrix[i][j] = functionWithoutDim(samples->getPoint(i), samples->getPoint(j), dim, samples->getDim()) * samples->getPoint(i)->y * samples->getPoint(j)->y;
            matrix[j][i] = matrix[i][j];
        }
    return matrix;
}

double Kernel::functionWithoutDim(std::shared_ptr<Point> one, std::shared_ptr<Point> two, int j, int dim) {
    register int i = 0;
    register double t, sum = 0.0;

    switch(type)
    {
        case 0: //Produto Interno
            for(i = 0; i < dim; ++i)
                if(i != j)
                    sum += one->x[i] * two->x[i];
            break;

        case 1: //Polinomial
            for(i = 0; i < dim; ++i)
                if(i != j)
                    sum += one->x[i] * two->x[i];
            sum = (param > 1) ? pow(sum+1, param) : sum;
            break;

        case 2: //Gaussiano
            for(i = 0; i < dim; ++i)
                if(i != j)
                { t = one->x[i] - two->x[i]; sum += t * t; }
            sum = exp(-1 * sum * param);
            break;
    }
    /*The '+1' here accounts for the bias term "b" in SVM formulation since
      <w,x> = \sum_i \alpha_i y_i k(x_i,x) + b and b=\sum_i \alpha_i y_i*/
    return sum;// + 1.0f;
}
