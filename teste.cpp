#include "includes/Point.hpp"
#include "includes/Data.hpp"
#include "includes/Kernel.hpp"
#include "includes/Visualization.hpp"
#include "includes/Classifier.hpp"
#include "includes/PrimalClassifier.hpp"
#include "includes/DualClassifier.hpp"
#include "includes/Validation.hpp"
#include "includes/Perceptron.hpp"
#include "includes/IMA.hpp"

using namespace std;

int main(int argc, char *argv[]){
	shared_ptr<Data<double> > data(make_shared<Data<double> >());
	size_t i, size, pos = 0, neg = 0;
	Kernel K;

	data->load(argv[1]);
	size = data->getSize();
	
	cout << "Data loaded." << endl;
	K.setType(0);
	
	clock_t begin = clock();
	IMADual<double> ima_dual(data, &K, 1, nullptr);

	ima_dual.setVerbose(0);
	ima_dual.train();
	cout << "Classifier trained." << endl;
	clock_t end = clock();

	for(i = 0; i < size; i++){
		if(ima_dual.evaluate(*((*data)[i])) > 0){
			pos++;
		}else neg++;
	}
	
	cout << pos << " positives." << endl;
	cout << neg << " negatives." << endl;
}

