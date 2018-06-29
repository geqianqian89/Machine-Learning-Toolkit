#include "includes/Point.hpp"
#include "includes/Data.hpp"
#include "includes/Kernel.hpp"
#include "includes/Visualization.hpp"
#include "includes/Validation.hpp"
#include "includes/Perceptron.hpp"
#include "includes/IMA.hpp"
#include "includes/RFE.hpp"

using namespace std;

int main(int argc, char *argv[]){
	shared_ptr<Data<double> > data(make_shared<Data<double> >()), fdata;
	IMAp<double> imap(data);
	Validation<double>::CrossValidation *cv = new Validation<double>::CrossValidation; 
	RFE<double> rfe(data, &imap, cv, 2, 1, 1);
	size_t i, size;
	
	clog << "Loading dataset..." << endl;
	data->load(argv[1]);
	size = data->getSize();
	
	clog << "Starting feature selection." << endl;
	clock_t begin = clock();
	rfe.setVerbose(1);
	fdata = rfe.selectFeatures();
	clog << "Features Selected." << endl;
	clock_t end = clock();

	vector<int> fnames = fdata->getFeaturesNames();

	for(auto itr = fnames.begin(); itr != fnames.end(); itr++){
		cout << *itr << " ";
	}
	cout << endl;
	
}

