#include "../includes/Point.hpp"
#include "../includes/Data.hpp"

using namespace std;

int main(){
	Data<float> data;
    data.load("iris.data");
    cout << data << endl;
}

