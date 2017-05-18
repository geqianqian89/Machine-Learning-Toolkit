#include <cmath>

#include "../includes/Kernel.hpp"

using namespace std;

Kernel::Kernel(){

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
