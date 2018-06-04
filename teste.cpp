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
	Data<double> data(argv[1]);
	Visualization<double> plot(&data);
	PerceptronPrimal<double> perceptron(std::make_shared<Data<double> >(data));
	Solution sol;

	perceptron.train();
	sol = perceptron.getSolution();

	plot.plot3DwithHyperplane(1, 2, 3, sol);
	while(true);
}

