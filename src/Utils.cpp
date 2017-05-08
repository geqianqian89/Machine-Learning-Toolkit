/*! Utils functions
   \brief Implementation of methods for general use in the system.
   \file Utils.cpp
   \author Mateus Coutinho Marim
*/

#include <cmath>
#include "../includes/Utils.hpp"

using namespace std;

bool is_number(string str){
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

int stoin(string str){
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

double stodn(string str){
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

double maxAbsElement(vector<double> x){
    int i, dim = x.size();
    double max, absv;

    for(i = 0, max = -INF; i < dim; i++){
        absv = fabs(x[i]);
        if(absv > max){
            max = absv;
        }
    }

    return max;
}
