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
	Timer time;
	Data<unsigned char> data(argv[1]);
	cout << time.Elapsed()/1000 << endl;
	Visualization<unsigned char> plot(&data);
	PerceptronPrimal<unsigned char> perceptron(std::make_shared<Data<unsigned char> >(data));
	Solution sol;

	perceptron.train();
	cout << perceptron.getElapsedTime()/1000 << endl;

	sol = perceptron.getSolution();
	/*for(int i = 0; i < sol.w.size(); i++){
		cout << sol.w[i] << " ";
	}*/
	cout << endl;
	plot.plot3DwithHyperplane(1, 2, 3, sol);
	while(true);
}

