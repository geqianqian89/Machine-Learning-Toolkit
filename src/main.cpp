#include "../includes/Point.hpp"
#include "../includes/Data.hpp"
#include "../includes/Kernel.hpp"
#include "../includes/Visualization.hpp"
#include "../includes/Classifier.hpp"
#include "../includes/PrimalClassifier.hpp"
#include "../includes/DualClassifier.hpp"
#include "../includes/Validation.hpp"
#include "../includes/Perceptron.hpp"
#include "../includes/IMA.hpp"

using namespace std;

int main(){
    Kernel k(2, 0.1);
	Data<double> data("DB/test.data");
	dMatrix *mat;
    Visualization<double> vis(&data);
    PerceptronPrimal<double> perc(make_shared<Data<double> >(data), 2, 1, nullptr);
    IMAp<double> ima(make_shared<Data<double> >(data), 0);
    Validation<double> val(make_shared<Data<double> >(data), &ima);

    perc.train();
    ima.setAlphaAprox(1);
    ima.setFlexible(0);
    ima.setpNorm(2);
    ima.setVerbose(3);
    ima.train();
    k.compute(data);
	mat = k.getKernelMatrixPointer();
	//val.validation(3,3);

	for(int i = 0; i < data.getSize(); i++){
        for(int j = 0; j < data.getSize(); j++){
            cout << (*mat)[i][j] << " ";
        }
        cout << endl;
	}
	Solution sol = perc.getSolution();
	cout << data << endl;

	for (int i; i < sol.w.size(); i++){
        cout << sol.w[i] << " ";
	}
	cout << sol.bias << endl;
	vis.plot2DwithHyperplane(1, 2, sol);

}

