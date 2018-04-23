#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cmath>
#include "data.h"
#include "embrapa.h"

using namespace std;

/*-----------------------------------------------------------------*
 * Load data from .txt file (classes (pta) for Embrapa database)   *
 *-----------------------------------------------------------------*/

void Embrapa::load_pta(Sample *samples, string ptaFile){
	string str, item;
	ifstream pta(ptaFile.c_str());
	int id = 0;

	if(!pta){
		cout << endl;
		cout << "File could not be opened!" << endl;
	}

	int n = 1;
	while(getline(pta, str)){
		istringstream ss(str);
		while(getline(ss, item, ' ')){
			if(n%2 == 0){
				//samples->points[id-1]->points[id-1]->
				samples->points[id-1]->original_pta = Data::stodn(item);
				samples->points[id-1]->has_pta = true;
			}else if(n%2 == 1){
				id = Data::stoin(item);
			}
			n++;
		}
	}

	pta.close();
}

/*----------------------------------------------------------*
 * Compute distance between points                          *
 *----------------------------------------------------------*/

Sample* Embrapa::correct_pta(Sample *samples, int option){
	int i, j, k, cont = 0, soma;
	vector<int> repeated_data, repeated;

	repeated_data = vector<int>(samples->size, 0);

	for(i = 0; i < samples->size; ++i){
		samples->points[i]->corrected_pta = samples->points[i]->original_pta;
	}

	for(i = 0; i < samples->size; ++i){
		for(j = 0; j < samples->size; ++j){
			if(i != j){
				for(soma = 0, k = 0; k < samples->dim; ++k){
					soma += fabs(samples->points[i]->x[k] - samples->points[j]->x[k]);

					if(soma > 0)
						break;
				}

				if(soma == 0){
					if(samples->points[j]->corrected_pta < samples->points[i]->corrected_pta)
						samples->points[i]->corrected_pta = samples->points[j]->corrected_pta;
					if(!repeated_data[i]){
						repeated_data[i] = 1;
						cont++;
					}
				}
			}
		}
	}

	if(option == 2){
		cout << "Total of erased ID's (equal points): " << cont << endl;
		repeated = vector<int>(cont, 0);

		for(j = 0, i = 0; i < samples->size; ++i){
			if(repeated_data[i] == 1){
				repeated[j++] = samples->points[i]->id;
				cout << repeated[j - 1] << " ";
			}
		}
		repeated.erase(unique(repeated.begin(), repeated.end()), repeated.end());
		samples = Data::remove_points_without_id(samples, repeated);
		cout << "\n" << endl;
	}

	for(i = 0; i < samples->size; ++i){
		Point *p = samples->points[i];

		cout << "id " << p->id << " original pta " << p->original_pta << " corrected pta " << p->corrected_pta << endl;
	}

	return samples;
}

/*----------------------------------------------------------*
 * Generate classes from pta                                *
 *----------------------------------------------------------*/

int Embrapa::generate_classes(Sample *samples, int option){
	int i, ind;
	double limit = 0;
	double average, percentage;
	vector<double> pta;

	switch(option){
		case 1: //pos. /neg.
				limit = 0;
				break;
		case 2: //average
				average = 0.0;

				for(i = 0; i < samples->size; ++i){
					average += samples->points[i]->corrected_pta;
				}

				limit = average/samples->size;
				break;
		case 3: //median
				pta = vector<double> (samples->size);

				for(i = 0; i < samples->size; ++i){
					pta[i] = samples->points[i]->corrected_pta;
				}

				sort(pta.begin(), pta.end());

				for(i = 0; i < samples->size; ++i){
					cout << pta[i] << endl;
				}

				ind = samples->size/2 - 1;
				limit = pta[ind];

				pta.clear();
				cout << "Limit: " << limit << endl;
				break;
		case 4: //Higher percentage
				cout << "Percentage (higher): ";
				cin >> percentage;

				pta = vector<double> (samples->size);

				for(i = 0; i < samples->size; ++i){
					pta[i] = samples->points[i]->corrected_pta;
				}

				sort(pta.begin(), pta.end());

				for(i = 0; i < samples->size; ++i){
					cout << pta[i] << endl;
				}

				ind = samples->size - samples->size*percentage/100 - 1;
				limit = pta[ind];

				pta.clear();
				cout << "limit: " << limit << endl;
				break;
		case 5: //Especific value
				cout << "Limit: ";
				cin >> limit;
				break;
		default:
				cout << "Invalid option!" << endl;
				break;
	}

	samples->npos_samples = 0;
	samples->nneg_samples = 0;

	for(i = 0; i < samples->size; ++i){
		if(samples->points[i]->corrected_pta > limit){
			samples->npos_samples++;
			samples->points[i]->y = 1;
		}else{
			samples->nneg_samples++;
			samples->points[i]->y = -1;
		}
	}

	return 1;
}

/*----------------------------------------------------------*
 * Erase points without PTA                                 *
 *----------------------------------------------------------*/

Sample* Embrapa::remove_points_without_pta(Sample *samples, int verbose){
	int i, j, cont = 0;
	vector<int> ids;

	if(verbose) cout << "IDs without PTA:\n";

	for(i = 0; i < samples->size; i++){
		if(!samples->points[i]->has_pta){
			cont++;
		}
	}
	cout << endl;

	ids.resize(cont, 0);

	for(j = 0, i = 0; i < samples->size; ++i){
		if(!samples->points[i]->has_pta){
			ids[j++] = samples->points[i]->id;
		}
	}
	if(verbose){
		for(i = 0; i < cont; ++i)
		    cout << ids[i] << " ";

	}
	cout << endl;
    cout << "\nTotal removed: " << cont << endl;

    samples = Data::remove_points_without_id(samples, ids);

	return samples;
}

