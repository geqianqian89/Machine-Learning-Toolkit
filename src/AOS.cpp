//
// Created by Mateus Coutinho Mari on 8/20/2018.
//

#include "../includes/AOS.hpp"

template<typename T>
AOS<T>::AOS(std::shared_ptr<Data<T>> samples, Classifier<T> *classifier, Validation<T>::CrossValidation *cv,
            int breadth, int depth, double bonus, int cut, int look_ahead_depth, int skip, int startover,
            int doleave_oo, int sorting_shape, int choice_shape, int verbose) {

}

template<typename T>
std::shared_ptr<Data<T>> AOS<T>::selectFeatures() {
    return std::shared_ptr<Data<T>>();
}

template class AOS<int>;
template class AOS<double>;
template class AOS<float>;
template class AOS<int8_t>;
template class AOS<char>;
template class AOS<long long int>;
template class AOS<short int>;
template class AOS<long double>;
template class AOS<unsigned char>;
template class AOS<unsigned int>;
template class AOS<unsigned short int>;