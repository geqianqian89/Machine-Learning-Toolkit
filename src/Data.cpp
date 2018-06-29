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
#include <utility>
#include <ctime>
#include <cmath>
#include <cstdio>

#include "../includes/Data.hpp"
#include "../includes/Utils.hpp"

using namespace std;

template < typename T >
Data< T >::Data(){}

template < typename T >
Data< T >::Data(const char* dataset, const char* pos_class, const char* neg_class){
    if(!load(string(dataset))){
        cerr << "Couldn't read the dataset." << endl;
    }
    this->pos_class = string(pos_class);
    this->neg_class = string(neg_class);
}

template < typename T >
Type Data< T >::identifyFileType(string file){
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

template < typename T >
bool Data< T >::load(string file){
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

template < typename T >
bool Data< T >::load_csv(string path){
    ifstream input(path.c_str());
    stringstream ss;
    string str, item;
    int dim, ldim, size, ssize;
    char deli;
    bool cond, flag, atEnd;

    atEnd = flag = false;
    dim = ldim = size = 0;

    if(!input){
        cout << "File could not be opened!" << endl;
        return false;
    }

    //Verify if the class is at the begining or at the end and error check
    while(getline(input, str)){
        ssize = str.size();
        dim = -1;
        ss.str(str);

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
        ss.clear();
    }

    input.clear();
    input.seekg(0, ios::beg);

    //initialize dim and size
    this->dim = dim;
    this->size = size;

    //reserve memory for fnames array and set feature names
    fnames.assign(dim, 0);
    index.assign(size, 0);
    iota(fnames.begin(), fnames.end(), 1);
    iota(index.begin(), index.end(), 0);

    //reserve memory for points array
    points.resize(size);
    size = 0;

    //Read sample (line) from file
    while(getline(input, str)){
        auto new_point = make_shared<Point< T > >();

        ss.str(str);
        dim = -1;

        //reserve memory for x array
        new_point->x.resize(this->dim, 0.0);

        //Read features from line
        while(getline(ss, item, deli)){
            if(atEnd)
                cond = (!ss.eof() && atEnd);
            else
                cond = !(dim == -1);

            if(cond){
                if(is_number(item))
                    new_point->x[(!atEnd)?dim:dim+1] = atof(item.c_str());
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
                new_point->y = c;
            }
            dim++;
        }

        points[size++] = std::move(new_point);
        points[size-1]->id = size;
        ss.clear();
    }

    is_empty = false;

    return true;
}

template < typename T >
bool Data< T >::load_data(string path){
    ifstream input(path.c_str());
    string str, item, buffer;
    stringstream ss;
    int dim, ldim, c, size;
    bool flag, atEnd = false, cond;

    if(!input){
        cout << "File could not be opened!" << endl;
        return false;
    }
    dim = ldim = size = c = 0;
    flag = false;

    //get dimension of the points
    while(getline(input, str)){
        dim = -1;

        ss.str(str);
        ss.clear();

        while(getline(ss, item, ' ')){
            if(!is_number(item)){
                clog << "Warning: point[" << size  << "] " << dim+1 << " feature is not a number." << endl;
                dim--;
            }
            //Verify if the class is at the beggining or at the end
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
    index.assign(size, 0);
    iota(fnames.begin(), fnames.end(), 1);
    iota(index.begin(), index.end(), 0);

    //reserve memory for points array
    points.resize(size);
    size = 0;

    //get lines from file
    while(getline(input, str)){
        auto new_point = make_shared<Point< T > >();

        ss.str(str);
        ss.clear();
        dim = 0;
        new_point->x.resize(this->dim, 0.0);

        //Read features from line
        while(getline(ss, item, ' ')){
            //Verify if the class is at the beggining or at the end
            if(atEnd)
                cond = (!ss.eof() && atEnd);
            else
                cond = !(dim == 0);

            if(cond){
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
                    new_point->x[(atEnd)?dim:dim-1] = atof(buffer.c_str());
                    dim++;
                }
            }else{
                c = (item == pos_class)?1:-1;
                new_point->y = c;
                if(c == -1) stats.n_neg++; else stats.n_pos++;

                if(!atEnd) dim++;
            }
        }
        //cout << points[150] << endl;
        points[size++] = std::move(new_point);
        points[size-1]->id = size;

    }

    input.close();
    is_empty = false;
    return true;
}

template < typename T >
bool Data< T >::load_arff(string path){
    ifstream input(path.c_str());
    istringstream ss;
    string str, item;
    int dim, ldim, size, c;
    bool atEnd, atBegin, flag, cond;

    if(!input){
        cout << "File could not be opened!" << endl;
        return false;
    }

    dim = ldim = size = c = 0;
    atEnd = atBegin = flag = cond = false;

    //Verify if the class is at the begining or at the end and error check
    while(getline(input, str)){
        dim = 0;
        ss.str(str);

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
        ss.clear();
    }
    input.clear();
    input.seekg(0, ios::beg);

    //initialize dim and size
    dim--;
    this->dim = dim;
    this->size = size;

    //reserve memory for fnames array and set feature names
    fnames.assign(dim, 0);
    index.assign(size, 0);
    iota(fnames.begin(), fnames.end(), 1);
    iota(index.begin(), index.end(), 0);

    //reserve memory for points array
    points.resize(size);

    size = 0;

    //Read line (sample) from file
    while(getline(input, str)){
        auto new_point = make_shared<Point< T > >();
        dim = -1;
        ss.str(str);

        //reserve memory for features
        new_point->x.assign(this->dim, 0.0);

        //Read features from line
        while(getline(ss, item, ',')){
            if(atEnd)
                cond = (!ss.eof() && atEnd);
            else
                cond = !(dim == 0);

            if(cond){
                if(is_number(item)){
                    new_point->x[dim + 1] = atof(item.c_str());
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
                new_point->y = c;
            }
            dim++;
        }

        points[size++] = std::move(new_point);
        points[size-1]->id = size;
        ss.clear();
    }

    is_empty = false;

    return true;
}

template < typename T >
bool Data< T >::load_txt(string path){
    ifstream input(path.c_str());
    istringstream ss;
    string str, item;
    int n, dim, n1, d, size;

    if(!input){
        cout << "File could not be opened!" << endl;
        return false;
    }

    n1 = d = size = 0;

    //error check
    while(getline(input, str)){
        ss.str(str);
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
        ss.clear();
    }

    //Initialize size and dim
    this->size = size;
    this->dim = dim;

    //Set features names
    fnames.assign(dim, 0);
    index.assign(size, 0);
    iota(fnames.begin(), fnames.end(), 1);
    iota(index.begin(), index.end(), 0);

    input.clear();
    input.seekg(0, ios::beg);

    //Allocate memory for points array
    points.resize(size);

    size = 0;

    //get line from file (sample)
    while(getline(input, str)){
        auto new_point = make_shared<Point< T > >();

        //Allocate memory for features
        new_point->x.resize(dim, 0.0);
        ss.str(str);
        n = 0;

        //read features from line
        while(getline(ss, item, ' ')){
            if(n >= 2){
                if(is_number(item))
                    new_point->x[n - 2] = atof(item.c_str());
                else{ clog << "Warning: point[" << size  << "] " << n-2 << " feature is not a number." << endl; }
                new_point->y = 0;
            }
            n++;
        }

        points[size++] = std::move(new_point);
        points[size-1]->id = size;
        ss.clear();
    }

    is_empty = false;

    return true;
}

template < typename T >
bool Data< T >::removePoint(int pid){
    int i;

    if(size == 1){ cout << "Error: RemovePoint, only one point left\n"; return false; }
    //Ids bound verification
    if(pid > points[size-1]->id || pid <= 0) return false;

    if(index.size() != 0){
        index.resize(size);

        for(i = 0; i < size; ++i){
            if(i >= pid){
                index[i-1] = index[i] - 1;
            }
        }
    }

    //Find the point by its id and erase it
    for(i = 0; i < size; i++){
        if(points[i]->id == pid){
            if(stats.n_pos > 0 || stats.n_neg > 0){
                if(points[i]->y == 1) stats.n_pos--;
                else if(points[i]->y == -1) stats.n_neg--;
            }
            points.erase(points.begin() + i);
            break;
        }
    }

    size--;

    return true;
}

template < typename T >
void Data< T >::write(string fname, string ext){
    int i, j;
    string path = fname + "." + ext;
    ofstream outstream(path.c_str(), ios::out);

    if(!outstream.is_open()){
        cerr << "Can't write in file." << endl;
        return;
    }

    for(i = 0; i < size; i++){
        outstream << points[i]->y << " ";

        if(ext == "plt"){
            for(j = 0; j < dim-1; j++){
                outstream << points[i]->x[j] << " ";
            }
            outstream << points[i]->x[j] << endl;
        }else if(ext == "data"){

            for(j = 0; j < dim-1; j++){
                outstream << fnames[j] << ":" << points[i]->x[j] << " ";
            }
            outstream << fnames[j] << ":" << points[i]->x[j] << "\n";
        }
    }

    outstream.close();
}

template < typename T >
vector<bool> Data< T >::removePoints(vector<int> ids){
    int idsize = ids.size(), i;
    bool save;
    shared_ptr<Point< T > > po;
    auto p = points.begin();
    vector<bool> notFound(idsize, true);

    for(; p != points.end();){
        save = true;
        po = (*p);
        for(i = 0; i < idsize; i++){
            if(po->id == ids[i]){
                save = false;
                notFound[i] = false;
                break;
            }
        }

        if(save) p++;
        else{
            p = points.erase(p);
            //Size verification.
            if(size == 1){ clog << "Error: RemovePoint, only one point left." << endl; break;}
            if(po->y == 1) stats.n_pos--;
            else if(po->y == -1) stats.n_neg--;
            size--;
        }
    }

    return notFound;
}

template < typename T >
Data< T >* Data< T >::insertFeatures(std::vector<int> ins_feat){
    size_t i, j, s, offset = 0, fsize = ins_feat.size();
    bool saveflag;
    vector<int> new_fnames(fsize, 0);
    shared_ptr<Point< T > > p;
    Data< T > *smout = new Data< T >;

    if(fsize == 0) return this;
    sort(ins_feat.begin(), ins_feat.end());

    //error check
    if(fsize > dim){ cerr << "Error: InsertFeature, fsize(" << ins_feat.size() << ")>dim(" << dim << ")\n"; return smout; }
    smout->setDim(fsize);

    //Copying information to new data array
    for(i = 0; i < size; i++){
        p = make_shared<Point< T > >();
        p->x.resize(fsize);
        p->alpha = points[i]->alpha;
        p->id = points[i]->id;
        p->y = points[i]->y;

        //Copying features
        s = 0, offset = 0;
        for(j = 0; j < dim; j++){
            if(offset < fsize && fnames[j] == ins_feat[offset]){
                saveflag = true;
                offset++;
            }

            if(saveflag){
                p->x[s] = points[i]->x[j];
                new_fnames[s] = fnames[j];
                s++;
                saveflag = false;
            }
        }
        //error check
        if(s != fsize){
            cerr << "Error: Something went wrong on InsertFeature\n";
            cerr << "s = " << s << ", dim = " << dim << ", fsize = " << fsize << endl;
            smout->clear();
            return smout;
        }
        smout->insertPoint(p);
    }
    smout->setFeaturesNames(new_fnames);

    return smout;
}

template < typename T >
bool Data< T >::removeFeatures(std::vector<int> feats){
    size_t i, j, k, psize = points.size(), rsize = feats.size();
    typename vector< T >::iterator itr;
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
        for(itr = points[i]->x.begin(),k = 0, j = 0; itr != points[i]->x.end();){
            if(k == rsize) break;
            if(fnames[j] == feats[k]){
                if(i == 0)  dim--;
                itr = points[i]->x.erase(itr);
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

template < typename T >
bool Data< T >::insertPoint(Data< T > sample, int index){
    if(index > sample.getSize()-1){
        cerr << "Index out of bounds. (insertPoint)" << endl;
        return false;
    }

    insertPoint(sample.getPoint(index));

    return true;
}

template < typename T >
bool Data< T >::insertPoint(shared_ptr<Point< T > > p){
    //Dimension verification
    if(int(p->x.size()) > dim){
        cerr << "Point with dimension different from the data. (insertPoint)" << endl;
        cerr << "Point dim = " << p->x.size() << " dim = " << dim << endl;
        return false;
    }

    //Insert the point p at the end of the points vector
    points.insert(points.end(), std::move(p));
    size++;
    if(is_empty) is_empty = false;

    if(points[size-1]->y > 0)
        stats.n_pos++;
    else stats.n_neg++;

    //Give a new id to the point equal to the previous point id plus 1
    points[size-1]->id = size;
    index.push_back(size-1);

    return true;
}

template < typename T >
void Data< T >::changeXVector(vector<int> index){
    int i, j;
    vector<shared_ptr<Point< T > > > nPoints(size);

    //Copy features and classes of the points making the changes
    for(i = 0; i < size; i++){
        nPoints[i]->x = points[index[i]]->x;
        nPoints[i]->x = points[index[i]]->x;
    }

    //Save changes in the class
    points = nPoints;
}

template < typename T >
shared_ptr<Point< T > > Data< T >::getPoint(int index){
    return points[index];
}

template < typename T >
void Data< T >::setPoint(int index, shared_ptr<Point< T > > p){
    points[index] = p;
}

template < typename T >
Data< T > Data< T >::copy(){
    return *this;
}

template < typename T >
void Data< T >::copyZero(const Data< T >& other){
    fnames = other.fnames;
    dim = other.dim;
    size = 0;
    is_empty = other.is_empty;
    normalized = other.normalized;
}

template < typename T >
void Data< T >::join(std::shared_ptr<Data< T > > data){
    size_t i, j, dim1 = data->getDim(), antsize = size, size1 = data->getSize();
    vector<int> index1 = data->getIndex(), antindex = index;
    vector<shared_ptr<Point< T > > > points1 = data->getPoints();

    if(dim > dim1){
        cerr << "Error: sample1 dimension must be less or equal to sample2\n";
        exit(1);
    }

    size += size1;

    if(!index.empty() && !index1.empty()){
        index.resize(size, 0);
        for(i = 0; i < antsize; i++) index[i] = antindex[i];
        for(i = 0; i < size1; i++) index[i + antsize] = index1[i];
    }

    points.resize(size);

    for(i = antsize, j = 0; i < size && j < size1; i++, j++){
        points[i] = points1[j];
        if(points1[j]->y == 1) stats.n_pos++;
        else if(points1[j]->y == -1) stats.n_neg++;
    }

}

template < typename T >
void Data< T >::normalize(double p){
    int i = 0, j = 0;
    double norm = 0.0;

    for(i = 0; i < size; ++i){
        for(norm = 0, j = 0; j < dim; ++j){
            norm += pow(fabs(points[i]->x[j]),p);
        }
        points[i]->x.resize(dim+1);
        points[i]->x[j] = 1;
        fnames[j] = j+1;
        norm += pow(fabs(points[i]->x[j]),p);
        norm = pow(norm, 1.0/p);
        for(j = 0; j < dim+1; ++j){
            points[i]->x[j] /= norm;
        }
    }
    dim++;

    normalized = true;
}

template < typename T >
void Data< T >::normalize(vector<double> &v, double q){
    size_t i = 0, dim = v.size();
    double norm = 0.0;

    for(i = 0; i < dim; ++i)
        norm += pow(fabs(v[i]), q);

    norm = pow(norm, 1.0/q);

    for(i = 0; i < dim; ++i)
        v[i] /= norm;
}

template < typename T >
void Data< T >::setDim(size_t dim){
    this->dim = dim;
}

template < typename T >
vector<int> Data< T >::getFeaturesNames(){
    return fnames;
}

template < typename T >
void Data< T >::setFeaturesNames(std::vector<int> fnames){
    this->fnames = fnames;
}

template < typename T >
vector<shared_ptr<Point< T > > > Data< T >::getPoints(){
    return points;
}

template < typename T >
vector<int> Data< T >::getIndex(){
    return index;
}

template < typename T >
int Data< T >::getNumberNegativePoints(){
    return stats.n_neg;
}

template < typename T >
int Data< T >::getNumberPositivePoints(){
    return stats.n_pos;
}

template < typename T >
Statistics< T > Data< T >::getStatistics(){
    return stats;
}

template < typename T >
void Data< T >::setClasses(string pos, string neg){
    pos_class = pos;
    neg_class = neg;
}

template < typename T >
bool Data< T >::isEmpty(){
    return is_empty;
}

template < typename T >
void Data< T >::operator=(const Data< T >& data){
    points = data.points;
    fnames = data.fnames;
    index = data.index;
    size = data.size;
    dim = data.dim;
    time_mult = data.time_mult;
    pos_class = data.pos_class;
    neg_class = data.neg_class;
    is_empty = data.is_empty;
    normalized = data.normalized;
    stats = data.stats;
}


template < typename T >
void Data< T >::clear(){
    points.clear();
    fnames.clear();
    index.clear();
    size = 0;
    dim = 0;
    stats.n_neg = 0;
    stats.n_pos = 0;
    stats.centroid = Point< T >();
    stats.neg_centroid = Point< T >();
    stats.pos_centroid = Point< T >();
    normalized = false;
    is_empty = true;
}

template < typename T >
Data< T >::~Data(){
    this->points.clear();
    this->index.clear();
    this->fnames.clear();
}

template < typename T >
void Data< T >::setIndex(std::vector<int> index) {
    this->index = index;
}

template < typename T >
void Data< T >::resetIndex(){
    index.assign(size, 0);
    iota(index.begin(), index.end(), 0);
}

template < typename T >
double Data< T >::getTime_mult() const {
    return time_mult;
}

template < typename T >
bool Data< T >::operator==(const Data< T > &rhs) const {
    return points == rhs.points &&
           fnames == rhs.fnames &&
           index == rhs.index &&
           size == rhs.size &&
           dim == rhs.dim &&
           time_mult == rhs.time_mult &&
           pos_class == rhs.pos_class &&
           neg_class == rhs.neg_class &&
           is_empty == rhs.is_empty &&
           normalized == rhs.normalized;
}

template < typename T >
bool Data< T >::operator!=(const Data< T > &rhs) const {
    return !(rhs == *this);
}

template class Data<int>;
template class Data<double>;
template class Data<float>;
template class Data<int8_t>;
template class Data<char>;
template class Data<long long int>;
template class Data<short int>;
template class Data<long double>;
template class Data<unsigned char>;
template class Data<unsigned int>;
template class Data<unsigned short int>;
