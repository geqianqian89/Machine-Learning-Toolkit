//
// Created by Mateus Coutinho Mari on 8/20/2018.
//

#ifndef UFJF_MLTK_AOS_H
#define UFJF_MLTK_AOS_H

#include "FeatureSelection.hpp"

template < typename T >
class AOS: public FeatureSelection< T >{
private:
    struct select_weight
    {
        int fname;
        int indice;
        double w;
        double val;
        double pmargin;
        double raio;
        double dcents;
        double golub;
        double fisher;
    };

    /* Gamma Struct: Used to stack margin values*/
    struct select_gamma
    {
        std::vector<int> fnames;
        int level;
        int sv;
        int train;
        double value; /*valor usado como criterio de escolha*/
        double pgamma; /*projected gamma*/
        double rgamma; /*real gamma p/ display*/
        double raio; /*raio*/
        double dcents; /*distancia entre os centros*/
        double golub; /*golub - estatistica*/
        double fisher; /*fisher - estatistica*/
        std::vector<double> w;
        double bias;
    };

    struct select_hash
    {
        int length;
        int width;
        struct select_gamma ***elements;
    };

    struct select_heap
    {
        int size;
        struct select_gamma **elements;
    };
public:
    AOS(std::shared_ptr<Data< T > > samples = nullptr, Classifier< T > *classifier = nullptr, typename Validation< T >::CrossValidation *cv = nullptr,
        int breadth = 0, int depth = 0, double bonus = 0, int cut = 0, int look_ahead_depth = 0, int skip = 0,
        int startover = 0, int doleave_oo = 0, int sorting_shape = 0, int choice_shape = 0, int verbose = 0);
    std::shared_ptr<Data< T > > selectFeatures() override;
};


#endif //UFJF_MLTK_AOS_H
