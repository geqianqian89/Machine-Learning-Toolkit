#include <random>
#include <functional>
#include <memory>

#include "../includes/DualClassifier.hpp"
#include "../includes/Validation.hpp"
#include "../includes/Data.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Point.hpp"
#include "../includes/Random.hpp"

using namespace std;

Validation::Validation(Data *sample, Classifier *classifier){
    this->sample = sample;
    this->classifier = classifier;
}

void Validation::partTrainTest(int fold, unsigned int seed){
    int i, j, npos, nneg, size = sample->getSize();
    Point p, aux;
    Data sample_pos, sample_neg;

    sample_pos.copyZero(*sample);
    sample_neg.copyZero(*sample);

    for(i = 0; i < size; i++){
        p = sample->getPoint(i);
        if(p.y == 1){
            sample_pos.insertPoint(*sample, i);
        }else{
            sample_neg.insertPoint(*sample, i);
        }
    }

    npos = sample_pos.getSize();
    nneg = sample_neg.getSize();

    auto dice_pos = bind(uniform_int_distribution<int>(0,npos-1), mt19937(seed));
    auto dice_neg = bind(uniform_int_distribution<int>(0,nneg-1), mt19937(seed));

    for(i = 0; i < npos; i++){
        j = dice_pos();
        aux = sample_pos.getPoint(i);
        sample_pos.setPoint(i, sample_pos.getPoint(j));
        sample_pos.setPoint(j, aux);
    }
    for(i = 0; i < nneg; i++){
        j = dice_neg();
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
  unique_ptr<Data> sample_pos(new Data), sample_neg(new Data), train_sample, test_sample, traintest_sample;
  vector<unique_ptr<Data> > vet_sample_pos(fold), vet_sample_neg(fold), vet_sample_final(fold);

/*  if(dynamic_cast<DualClassifier*>(classifier) != nullptr){
    kernel_type = classifier->getType();
    kernel_param = classifier->getParam();
  }*/

  sample_neg->copyZero(*sample);
  sample_pos->copyZero(*sample);

  for(i = 0; i < size; i++){
    Point *p = sample->getPtrToPoint(i);
    if(p->y == 1)
			sample_pos->insertPoint(*p);
		else
			sample_neg->insertPoint(*p);
  }
  cout << *sample_neg << "\n" << *sample_pos << endl;
  qtdpos = sample_pos->getSize();
	qtdneg = sample_neg->getSize();

	cout << "\nTotal of points: " << sample->getSize() << endl;
	cout << "Qtd of positive: " << qtdpos << endl;
	cout << "Qtd of negative: " << qtdneg << endl;

  Random::init();
	//randomize
	for(i = 0; i < qtdpos; ++i){
		Point aux;

		j = Random::intInRange(0, sample_pos->getSize()-1);
		aux = *sample_pos->getPtrToPoint(i);
		*sample_pos->getPtrToPoint(i) = *sample_pos->getPtrToPoint(j);
		*sample_pos->getPtrToPoint(j) = aux;
	}

	for(i = 0; i < qtdneg; ++i){
		Point aux;

		j = Random::intInRange(0, sample_neg->getSize()-1);
		aux = *sample_neg->getPtrToPoint(i);
		*sample_neg->getPtrToPoint(i) = *sample_neg->getPtrToPoint(j);
		*sample_neg->getPtrToPoint(j) = aux;
	}


	for(i = 0; i < fold; ++i){
    vet_sample_pos[i].reset(new Data);
		vet_sample_neg[i].reset(new Data);
		vet_sample_final[i].reset(new Data);

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



}

void Validation::validation(int fold, int qtde){

}

Data Validation::getTestSample(){
    return test_sample;
}

Data Validation::getTrainSample(){
    return train_sample;
}
