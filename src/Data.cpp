/**
  \brief Implementation of the Data class methods.
  \file Data.cpp
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <iterator>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdio>

#include "../includes/Data.hpp"
#include "../includes/Utils.hpp"

using namespace std;

Data::Data(){

}

Data::Data(string dataset){
    if(!load(dataset)){
        cerr << "Couldn't read the dataset." << endl;
    }
}

Type Data::identifyFileType(string file){
    int l , k, i, j, tsize = types.size();

        for(k = file.size() - 1; (k >= 0 && file[k] != '.') ; k--){
            for(i = 0; i < tsize; ++i){
                if(file[k] == types[i][types[i].size()-1]){
                    for(j = types[i].size() - 1, l = 0; (j >= 0 && file[k-l] != '.') ; j--, l++){
                        if(j == 0 && types[i][j] == file[k-l]){
                            switch(i){
                                case 0:
                                        return Type::TYPE_DATA;
                                        break;
                                case 1:
                                        return Type::TYPE_CSV;
                                        break;
                                case 2:
                                        return Type::TYPE_ARFF;
                                        break;
                                case 3:
                                        return Type::TYPE_TXT;
                                        break;
                                default:
                                        return Type::TYPE_INVALID;
                                        break;
                            }
                        }else if(types[i][j] != file[k-l]){
                            break;
                        }
                    }
                }
            }
        }

        return Type::TYPE_INVALID;
}

bool Data::load(string file){
    Type t = identifyFileType(file);

    switch (t) {
        case TYPE_ARFF:
            return load_arff(file);
        case TYPE_CSV:
            return load_csv(file);
        case TYPE_DATA:
            return load_data(file);
        case TYPE_TXT:
            return load_txt(file);
        default:
            cerr << "Invalid file type." << endl;
            return false;
    }

    return true;
}

bool Data::load_csv(string path){
    ifstream input(path.c_str());
    string str, item, pos_class, neg_class;
    int dim, ldim, size, ssize;
    char deli;
    bool cond, flag, atEnd;

    atEnd = flag = false;
    dim = ldim = size = 0;

    if(!input){
        cout << "File could not be opened!" << endl;
        return false;
    }

    cout << "Positive class: ";
    cin >> pos_class;
    cout << "Negative class: ";
    cin >> neg_class;

    //Verify if the class is at the begining or at the end and error check
    while(getline(input, str)){
        istringstream ss(str);
        ssize = str.size();
        dim = -1;

        //Define csv file delimitator
        if(size == 0){
            for(int i = 0; i < ssize; ++i){
                if(str[i] == ','){
                    deli = ',';
                    break;
                }else if(str[i] == ';'){
                    deli = ';';
                    break;
                }
            }
        }

        while(getline(ss, item, deli)){
            //check for invalid feature or class
            if(!is_number(item) && (item != pos_class || item != neg_class)){
                clog << "Warning: point[" << size  << "] " << dim+1 << " feature is not a number." << endl;
                dim--;
            }

            if(dim == -1 && !flag){
                if(!((item == pos_class) || (item == neg_class))){
                    atEnd = true;
                    flag = true;
                }
            }else if(ss.eof() && !flag){
                if(!((item == pos_class) || (item == neg_class))){
                    flag = true;
                }
            }
            dim++;
        }

        if(ldim != dim && ldim != 0){
            cerr << "All the samples must have the same dimension!" << endl;
            return false;
        }

        ldim = dim;
        size++;
    }

    input.clear();
    input.seekg(0, ios::beg);

    //initialize dim and size
    this->dim = dim;
    this->size = size;

    //reserve memory for fnames array and set feature names
    fnames.assign(dim, 0);
    iota(fnames.begin(), fnames.end(), 1);

    //reserve memory for points array
    points.resize(size);

    size = 0;

    //Read sample (line) from file
    while(getline(input, str)){
        Point new_point;
        istringstream ss(str);

        dim = -1;

        //reserve memory for x array
        new_point.x.resize(this->dim, 0.0);

        //Read features from line
        while(getline(ss, item, deli)){
            if(atEnd)
                cond = (!ss.eof() && atEnd);
            else
                cond = !(dim == -1);

            if(cond){
                if(is_number(item))
                    new_point.x[dim] = stodn(item);
            }else{
                int c;
                if(is_number(item)){
                    c = (item == pos_class)?1:-1;
                }else{
                    c = (item == pos_class)?1:-1;
                    if(item != pos_class){
                        clog << "Warning: point " << size << " class is not a number and it's not defined." << endl;
                        c = 0;
                    }
                }
                if(c == -1) stats.n_neg++; else stats.n_pos++;
                new_point.y = c;
            }
            dim++;
        }

        points[size++] = new_point;
        points[size-1].id = size;
    }

    is_empty = false;
    input.close();

    return true;
}

bool Data::load_data(string path){
    ifstream input(path.c_str());
    string str, item, pos_class, neg_class, buffer;
    int dim, ldim, c, size;
    bool flag;

    if(!input){
        cout << "File could not be opened!" << endl;
        return false;
    }

    cout << "Positive class: ";
    cin >> pos_class;
    cout << "Negative class: ";
    cin >> neg_class;

    dim = ldim = size = c = 0;
    flag = false;

    //get dimension of the points
    while(getline(input, str)){
        stringstream ss(str);
        dim = -1;

        while(getline(ss, item, ' ')){
            if(!is_number(item)){
                clog << "Warning: point[" << size  << "] " << dim+1 << " feature is not a number." << endl;
                dim--;
            }

            dim++;
        }

        if(ldim != dim && ldim != 0){
            cerr << "All the samples must have the same dimension!" << endl;
            return false;
        }

        ldim = dim;
        size++;
    }

    input.clear();
    input.seekg(0, ios::beg);

    //initialize dim and size
    this->dim = dim;
    this->size = size;

    //reserve memory for fnames array and set feature names
    fnames.assign(dim, 0);
    iota(fnames.begin(), fnames.end(), 1);

    //reserve memory for points array
    points.resize(size);

    size = 0;

    //get lines from file
    while(getline(input, str)){
        Point new_point;
        stringstream ss(str);

        dim = -1;
        new_point.x.resize(this->dim, 0.0);

        //Read features from line
        while(getline(ss, item, ' ')){
            if(dim == -1){
                c = (item == pos_class)?1:-1;
                new_point.y = c;
                if(c == -1) stats.n_neg++; else stats.n_pos++;
                dim++;
            }else{
                buffer.clear();

                flag = false; //Verify if it's including value or fname

                for(char i: item){	//Get feature name and value
                    if(i != ' ' && i != '\n'){
                        if(i != ':' && !flag){
                            buffer.push_back(i);
                        }else if(flag){
                            buffer.push_back(i);
                        }else if(i == ':' && i > 0 && this->size == 0){	//Get features names in the first running
                            fnames[dim] = stoin(buffer);
                            flag = true;
                            buffer.clear();
                        }else if(i == ':'){
                            buffer.clear();
                            flag = true;
                        }
                    }
                }

                if(is_number(buffer)){
                    new_point.x[dim] = stodn(buffer);
                    dim++;
                }
            }
        }
        points[size++] = new_point;
        points[size-1].id = size;
    }

    is_empty = false;
    input.close();

    return true;
}

bool Data::load_arff(string path){
    ifstream input(path.c_str());
    string str, item, pos_class, neg_class;
    int dim, ldim, size, c;
    bool atEnd, atBegin, flag, cond;

    if(!input){
        cout << "File could not be opened!" << endl;
        return false;
    }

    cout << "Positive class: ";
    cin >> pos_class;
    cout << "Negative class: ";
    cin >> neg_class;

    dim = ldim = size = c = 0;
    atEnd = atBegin = flag = cond = false;

    //Verify if the class is at the begining or at the end and error check
    while(getline(input, str)){
        istringstream ss(str);
        dim = 0;

        while(getline(ss, item, ',')){
            //check for invalid feature or class
            if(!is_number(item) && (item != pos_class || item != neg_class)){
                clog << "Warning: point[" << size  << "] " << dim+1 << " feature is not a number." << endl;
                dim--;
            }

            if(dim == 0 && !flag){
                if(!((item == pos_class) || (item == neg_class))){
                    atEnd = true;
                    flag = true;
                }
            }else if(ss.eof() && !flag){
                if(!((item == pos_class) || (item == neg_class))){
                    atBegin = true;
                    flag = true;
                }
            }
            dim++;
        }

        if(ldim != dim && ldim != 0){
            cerr << "All the samples must have the same dimension!" << endl;
            return false;
        }

        ldim = dim;
        size++;
    }

    input.clear();
    input.seekg(0, ios::beg);

    //initialize dim and size
    dim--;
    this->dim = dim;
    this->size = size;

    //reserve memory for fnames array and set feature names
    fnames.assign(dim, 0);
    iota(fnames.begin(), fnames.end(), 1);

    //reserve memory for points array
    points.resize(size);

    size = 0;

    //Read line (sample) from file
    while(getline(input, str)){
        istringstream ss(str);
        Point new_point;
        dim = -1;

        //reserve memory for features
        new_point.x.assign(this->dim, 0.0);

        //Read features from line
        while(getline(ss, item, ',')){
            if(atEnd)
                cond = (!ss.eof() && atEnd);
            else
                cond = !(dim == 0);
            if(cond){
                if(is_number(item)){
                    new_point.x[dim + 1] = stodn(item);
                }
            }else{
                if(is_number(item)){
                    c = (stoin(item) == stoin(pos_class))?1:-1;
                }else{
                    c = (item == pos_class)?1:-1;
                    if(item != pos_class){
                        clog << "Warning: point " << size << " class is not a number and it's not defined." << endl;
                        c = 0;
                    }
                }
                if(c == -1) stats.n_neg++; else stats.n_pos++;
                new_point.y = c;
            }
            dim++;
        }

        points[size++] = new_point;
        points[size-1].id = size;
    }

    is_empty = false;
    input.close();
    return true;
}

bool Data::load_txt(string path){
    ifstream input(path.c_str());
    string str, item;
    int n, dim, n1, d, size;

    if(!input){
        cout << "File could not be opened!" << endl;
        return false;
    }

    n1 = d = size = 0;

    //error check
    while(getline(input, str)){
        istringstream ss(str);
        n1 = 0;
        dim = 0;

        while(getline(ss, item, ' ')){
            if(n1 >= 2){
                dim++;
            }
            n1++;
        }

        if(size != 0 &&  dim != d ){
                cerr << dim << " " << this->dim << endl;
                cerr << "All the samples must have the same dimension!" << endl;
                return false;
        }

        d = dim;
        size++;
    }

    //Initialize size and dim
    this->size = size;
    this->dim = dim;

    //Set features names
    fnames.assign(dim, 0);
    iota(fnames.begin(), fnames.end(), 1);

    input.clear();
    input.seekg(0, ios::beg);

    //Allocate memory for points array
    points.resize(size);

    size = 0;

    //get line from file (sample)
    while(getline(input, str)){
        Point new_point;
        istringstream ss(str);
        n = 0;

        ss.clear();
        ss.str(str);

        //Allocate memory for features
        new_point.x.resize(dim, 0.0);

        //read features from line
        while(getline(ss, item, ' ')){
            if(n >= 2){
                if(is_number(item))
                    new_point.x[n - 2] = stoin(item);
                else{ clog << "Warning: point[" << size  << "] " << n-2 << " feature is not a number." << endl; }
                new_point.y = 0;
            }
            n++;
        }

        points[size++] = new_point;
        points[size-1].id = size;
    }

    is_empty = false;
    input.close();

    return true;
}

bool Data::removePoint(int pid){
    int i;

    if(size == 1){ cout << "Error: RemovePoint, only one point left\n"; return false; }
    //Ids bound verification
    if(pid > points[size-1].id || pid <= 0) return false;

    if(index.size() != 0){
        index.resize(size);

        for(i = 0; i < size; ++i){
            if(i >= pid){
                index[i-1] = index[i] - 1;
            }
        }
    }

    for(i = 0; i < size; i++){
        if(points[i].id == pid){
            if(stats.n_pos > 0 || stats.n_neg > 0){
                if(points[i].y == 1) stats.n_pos--;
                else if(points[i].y == -1) stats.n_neg--;
            }
            points.erase(points.begin() + i);
            break;
        }
    }

    size--;

    return true;
}

vector<bool> Data::removePoints(vector<int> ids){
    int i = 0, j, idsize = ids.size();
    vector<bool> notFound(idsize, true);

    for(j = 0; j < size && i != idsize; j++){
        if(points[j].id == ids[i]){
            //Size verification.
            if(size == 1){ clog << "Error: RemovePoint, only one point left." << endl; break;}
            //Ids bound verification
            if(ids[i] > points[size-1].id || ids[i] < 0){ break;}
            if(stats.n_pos > 0 || stats.n_neg > 0){
                if(points[j].y == 1) stats.n_pos--;
                else if(points[j].y == -1) stats.n_neg--;
            }
            points.erase(points.begin() + j);
            notFound[i] = false;
            size--;
            j--;
            i++;
        }
    }

    return notFound;
}

bool Data::removeFeatures(std::vector<int> feats){
    int i, j, k, psize = points.size(), rsize = feats.size();
    vector<double>::iterator itr;
    vector<int>::iterator fitr;

    if(fnames.size() == 1){
        cerr << "Error: RemoveFeature, only one feature left.\n";
        return false;
    }
    if(feats.size() >= fnames.size()){
        cerr << "Error: RemoveFeature, more or equal features to remove than exist.\n";
        return false;
    }

    //Sort feats for remove features easily
    sort(feats.begin(), feats.end());

    //Remove features from each point
    for(i = 0; i < psize; i++){
        for(itr = points[i].x.begin(),k = 0, j = 0; itr != points[i].x.end();){
            if(k == rsize) break;
            if(fnames[j] == feats[k]){
                if(i == 0) dim--;
                itr = points[i].x.erase(itr);
                k++;
            }else itr++;
            j++;
        }
    }

    //remove names of non-existent features
    for(k = 0; k < rsize; k++){
        for(fitr = fnames.begin(); fitr != fnames.end();){
            if((*fitr) == feats[k]){
                fitr = fnames.erase(fitr);
            }else{
                fitr++;
            }
        }
    }

    return true;
}

Data Data::copy(){
    return *this;
}

void Data::operator=(const Data& data){
    points = data.points;
    fnames = data.fnames;
    dim = data.dim;
    size = data.size;
    stats = data.stats;
    is_empty = data.is_empty;
}

int Data::getDim(){
    return dim;
}

int Data::getSize(){
    return size;
}

vector<int> Data::getFeaturesNames(){
    return fnames;
}

vector<Point> Data::getPoints(){
    return points;
}

int Data::getNumberNegativePoints(){
    return stats.n_neg;
}

int Data::getNumberPositivePoints(){
    return stats.n_pos;
}

Statistics Data::getStatistics(){
    return stats;
}
