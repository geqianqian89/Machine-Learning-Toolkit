#include "../includes/Point.hpp"
#include "../includes/Data.hpp"

using namespace std;

int main(){
	Point<double> p(3, 5);
	
	cout << p.norm() << endl;
	cout << p.dot(Point<double>(3, 2).x) << endl;
	cout << (p == Point<double> (3, 5)) << endl;
}

