#include <memory>

#include "../includes/DualClassifier.hpp"
#include "../includes/Validation.hpp"
#include "../includes/Data.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Point.hpp"
#include "../includes/Random.hpp"
#include "../includes/Kernel.hpp"

using namespace std;

Validation::Validation(Data *sample, Classifier *classifier, unsigned int seed){
    this->sample = sample;
    this->classifier = classifier;

    Random::init(seed);
    seed = Random::getSeed();
}

void Validation::partTrainTest(int fold){
    int i, j, npos, nneg, size = sample->getSize();
    shared_ptr<Point> p, aux;
    Data sample_pos, sample_neg;

    sample_pos.copyZero(*sample);
    sample_neg.copyZero(*sample);

    for(i = 0; i < size; i++){
        p = sample->getPoint(i);
        if(p->y == 1){
            sample_pos.insertPoint(*sample, i);
        }else{
            sample_neg.insertPoint(*sample, i);
        }
    }

    npos = sample_pos.getSize();
    nneg = sample_neg.getSize();

    for(i = 0; i < npos; i++){
        j = Random::intInRange(0, npos-1);
        aux = sample_pos.getPoint(i);
        sample_pos.setPoint(i, sample_pos.getPoint(j));
        sample_pos.setPoint(j, aux);
    }
    for(i = 0; i < nneg; i++){
        j = Random::intInRange(0, nneg-1);
        aux = sample_neg.getPoint(i);
        sample_neg.setPoint(i, sample_neg.getPoint(j));
        sample_neg.setPoint(j, aux);
    }

    train_sample.copyZero(sample_pos);
    test_sample.copyZero(sample_pos);

    for(j = 0; j < npos*(fold-1)/fold; j++)
        train_sample.insertPoint(sample_pos, j);
    for(; j < npos; j++)
        test_sample.insertPoint(sample_pos, j);

    for(j = 0; j < nneg/fold; j++)
        test_sample.insertPoint(sample_neg, j);
    for(; j < nneg; j++)
        train_sample.insertPoint(sample_neg, j);
}

double Validation::kFold (int fold, int seed){
    int i = 0, j = 0, k = 0, size = sample->getSize();
    double error = 0.0, func = 0.0, margin = 0.0;
    int qtdpos = 0, qtdneg = 0, cost_pos = 0, cost_neg = 0, svcount = 0;
    vector<double> w;
    dMatrix matrix;
    vector<int> error_arr(fold);
    unique_ptr<Data> sample_pos(make_unique<Data> ()), sample_neg(make_unique<Data> ()),
            train_sample(make_unique<Data> ()), test_sample(make_unique<Data> ()),
            traintest_sample(make_unique<Data> ());
    vector<unique_ptr<Data> > vet_sample_pos(fold), vet_sample_neg(fold), vet_sample_final(fold);
    bool isPrimal = false;

    Random::init(seed);

    if(dynamic_cast<DualClassifier*>(classifier) == nullptr){
        isPrimal = true;
    }

    sample_neg->copyZero(*sample);
    sample_pos->copyZero(*sample);

    for(i = 0; i < size; i++){
        shared_ptr<Point> p = sample->getPoint(i);
        if(p->y == 1)
            sample_pos->insertPoint(p);
        else
            sample_neg->insertPoint(p);
    }

    qtdpos = sample_pos->getSize();
    qtdneg = sample_neg->getSize();

    if(verbose){
        cout << "\nTotal of points: " << sample->getSize() << endl;
        cout << "Qtd of positive: " << qtdpos << endl;
        cout << "Qtd of negative: " << qtdneg << endl;
    }

    //randomize
    for(i = 0; i < qtdpos; ++i){
        shared_ptr<Point> aux;

        j = Random::intInRange(0, sample_pos->getSize()-1);
        aux = sample_pos->getPoint(i);
        sample_pos->setPoint(i, sample_pos->getPoint(j));
        sample_pos->setPoint(j, aux);
    }

    for(i = 0; i < qtdneg; ++i){
        shared_ptr<Point> aux;

        j = Random::intInRange(0, sample_neg->getSize()-1);
        aux = sample_neg->getPoint(i);
        sample_neg->setPoint(i, sample_neg->getPoint(j));
        sample_neg->setPoint(j, aux);
    }

    for(i = 0; i < fold; ++i){
        vet_sample_pos[i] = make_unique<Data>();
        vet_sample_neg[i] = make_unique<Data>();
        vet_sample_final[i] = make_unique<Data>();

        vet_sample_pos[i]->copyZero(*sample);
        vet_sample_neg[i]->copyZero(*sample);
        vet_sample_final[i]->copyZero(*sample);
    }

    for(i = 0, j = 0; i < fold - 1; ++i){
        for(; j < (qtdpos-cost_pos)/(fold-i)+cost_pos; ++j)
            vet_sample_pos[i]->insertPoint(*sample_pos, j);
        cost_pos = j;
    }

    for(; j < qtdpos; ++j){
        vet_sample_pos[i]->insertPoint(*sample_pos, j);
    }

    for(i = 0, j = 0; i < fold-1; ++i){
        for(; j < (qtdneg-cost_neg)/(fold-i)+cost_neg; ++j)
            vet_sample_neg[fold-1-i]->insertPoint(*sample_neg, j);
        cost_neg = j;
    }

    for(; j < qtdneg; ++j)
        vet_sample_neg[fold-1-i]->insertPoint(*sample_neg, j);

    sample_pos.reset(nullptr);
    sample_neg.reset(nullptr);

    for(i = 0; i < fold; ++i){
        for(j = 0; j < vet_sample_pos[i]->getSize(); ++j)
            vet_sample_final[i]->insertPoint(*vet_sample_pos[i], j);
        for(; j < vet_sample_pos[i]->getSize() + vet_sample_neg[i]->getSize(); ++j)
            vet_sample_final[i]->insertPoint(*vet_sample_neg[i], j - vet_sample_pos[i]->getSize());
    }

    for(i = 0; i < fold; ++i){
        vet_sample_pos[i].reset(nullptr);
        vet_sample_neg[i].reset(nullptr);
    }

    vet_sample_pos.clear();
    vet_sample_neg.clear();

    //Start cross-validation
    for(j = 0; j < fold; ++j){
        *test_sample = vet_sample_final[j]->copy();
        train_sample->copyZero(*sample);

        for(i = 0; i < fold; ++i){
            if(i != j)
                for(k = 0; k < vet_sample_final[i]->getSize(); ++k)
                    train_sample->insertPoint(*vet_sample_final[i], k);
        }

        if(verbose){
            cout << "\nCross-Validation " << j + 1 << ": \n";
            cout << "Train points: " << train_sample->getSize() << endl;
            cout << "Test points: " << test_sample->getSize() << endl;
        }

        //training
        classifier->setSamples(sample);
        classifier->setVerbose(0);
        if(!classifier->train()){
            if(verbose)
                cerr << "Error at " << fold << "-fold: The convergency wasn't reached at the set " << j+1 << "!\n";
            continue;
        }

        Solution s = classifier->getSolution();

        if(isPrimal){
            for(i = 0; i < test_sample->getSize(); ++i){
                shared_ptr<Point> p = test_sample->getPoint(i);
                for(func = s.bias, k = 0; k < train_sample->getDim(); ++k)
                    func += s.w[k] * p->x[k];

                if(p->y * func <= 0){
                    if(verbose > 1)
                        cerr << "[" << i+1 << "x] function: " << func << ", y: " << p->y << endl;
                    error_arr[j]++;
                }else{
                    if(verbose > 1)
                        cerr << "[" << i+1 << "] function: " << func << ", y: " << p->y << endl;
                }
            }
        }else{
            DualClassifier *dual = dynamic_cast<DualClassifier*>(classifier);
            Kernel K(dual->getKernelType(), dual->getKernelParam());
            dMatrix matrix = K.getKernelMatrix();

            *traintest_sample = test_sample->copy();
            traintest_sample->join(*train_sample);

            for(i = 0; i < test_sample->getSize(); ++i){
                for(func = s.bias, k = 0; k < train_sample->getSize(); ++k)
                    func += train_sample->getPoint(k)->alpha * train_sample->getPoint(k)->y * matrix[k+test_sample->getSize()][i];

                if(test_sample->getPoint(i)->y * func <= 0){
                    if(verbose > 1)
                        cerr << "[" << i+1 << "x] function: " << func << ", y: " << test_sample->getPoint(i)->y << endl;
                    error_arr[j]++;
                }else{
                    if(verbose > 1)
                        cerr << "[" << i+1 << "] function: " << func << ", y: " << test_sample->getPoint(i)->y << endl;
                }
            }
        }
        if(verbose) cout << "Error " << j + 1 << ": " << error_arr[j] << " -- " << ((double)error_arr[j]/(double)vet_sample_final[j]->getSize())*100.0f << "%";
        error += ((double)error_arr[j]/(double)vet_sample_final[j]->getSize())*100.0f;

        train_sample = make_unique<Data>();
        test_sample = make_unique<Data>();
    }

    return (((double)error)/(double)fold);
}

double Validation::validation(int fold, int qtde){
    int i = 0, k = 0, erro = 0, svcount = 0, test_size = test_sample.getSize(),
            train_size = train_sample.getSize(), train_dim = train_sample.getDim();
    double error = 0, errocross = 0, func = 0.0, margin = 0, bias;
    vector<double> w;
    Data traintest_sample;
    bool isPrimal = false;

    if(dynamic_cast<DualClassifier*>(classifier) == nullptr){
        isPrimal = true;
    }

    sample = &train_sample;

    /*cross-validation*/
    if(qtde > 0)
    {
        for(errocross = 0, i = 0; i < qtde; i++)
        {
            if(verbose) cout << "\nExecucao " << i + 1 << " / " << qtde << ":\n";
            errocross += kFold(fold, i);
        }
        cout << "\n\nErro " << fold << "-Fold Cross Validation: " << errocross/qtde << "\n";
    }

    /*start final validation*/
    if(verbose)
    {
        cout << "\nFinal Validation:\n";
        cout << "Pts de Treino: " << train_sample.getSize() << "\n";
        cout << "Pts de Teste:  " << test_sample.getSize() << "\n";
    }

    //training
    classifier->setSamples(sample);
    classifier->setVerbose(0);
    if(!classifier->train()){
        if(verbose)
            cerr << "Validation error: The convergency wasn't reached in the training set!\n";
    }

    Solution s = classifier->getSolution();

    w = s.w;
    bias = s.bias;

    if(isPrimal){
        for(i = 0; i < test_size; ++i)
        {
            shared_ptr<Point> p = test_sample.getPoint(i);
            for(func = bias, k = 0; k < train_dim; ++k)
                func += w[k] * p->x[k];

            if(p->y * func <= 0)
            {
                if(verbose > 1) cout << "["<< i+1 <<"x] function: " << func << ", y: " << p->y  << "\n";
                erro++;
            }
            else
            {
                if(verbose > 1) cout << "["<< i+1 <<"x] function: " << func << ", y: " << p->y  << "\n";
            }
            if(verbose) cout.flush();
        }
    }else{
        /*testing imadual and smo*/
        DualClassifier *dual = dynamic_cast<DualClassifier*>(classifier);
        Kernel K(dual->getKernelType(), dual->getKernelParam());
        dMatrix matrix = K.getKernelMatrix();

        traintest_sample.join(test_sample);
        traintest_sample.join(train_sample);

        for(i = 0; i < test_size; ++i)
        {
            shared_ptr<Point> p = test_sample.getPoint(i);
            for(func = bias, k = 0; k < train_size; ++k)
                func += train_sample.getPoint(k)->alpha * train_sample.getPoint(k)->y * matrix[k+test_size][i];

            if(p->y * func <= 0)
            {
                if(verbose > 1) cout << "["<< i+1 <<"x] function: " << func << ", y: " << p->y  << "\n";
                erro++;
            }
            else
            {
                if(verbose > 1) cout << "["<< i+1 <<"x] function: " << func << ", y: " << p->y  << "\n";
            }
            if(verbose) cout.flush();
        }
    }

    cout << "Validation Error: " << erro << " -- " << (double)erro/(double)test_sample.getSize()*100.0f << "\n";
    error += ((double)erro/(double)test_sample.getSize())*100.0f;
}

Data Validation::getTestSample(){
    return test_sample;
}

Data Validation::getTrainSample(){
    return train_sample;
}
