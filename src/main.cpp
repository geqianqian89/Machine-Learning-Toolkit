#include "../includes/Point.hpp"
#include "../includes/Data.hpp"
#include "../includes/Kernel.hpp"
#include "../includes/Visualization.hpp"
#include "../includes/Classifier.hpp"
#include "../includes/PrimalClassifier.hpp"

using namespace std;

int main(){
    Kernel k(2, 0.1);
	Data<float> data("cmake-build-debug/DB/test.data");
	dMatrix *mat;
    Visualization<float> vis(&data);

    k.compute(data);
	mat = k.getKernelMatrixPointer();

	for(int i = 0; i < data.getSize(); i++){
        for(int j = 0; j < data.getSize(); j++){
            cout << (*mat)[i][j] << " ";
        }
        cout << endl;
	}
	cout << data << endl;
	cout << data[0]->norm(2) << endl;
	cout << (*data[0])[0] << endl;
	vis.plot2D(1, 2);

}

