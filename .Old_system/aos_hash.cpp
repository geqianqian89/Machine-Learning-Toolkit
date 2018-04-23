#include <iostream>
#include <vector>
#include <cmath>
#include "aos_hash.h"

using namespace std;

/*----------------------------------------------------------*
 * creates a fresh new hash                                 *
 *----------------------------------------------------------*/

AOS_Hash::AOS_Hash(int length, int width) {
    int i = 0, j = 0;

    this->length = length;
    this->width  = width;

    elements.resize(length);

    for(i = 0; i < length; ++i){
        elements[i].resize(width);

        //set elements as NULLS
        for(j = 0; j < width; ++j)
            elements[i][j] = NULL;
    }
}

bool AOS_Hash::node_equal(aos_select_gamma one, aos_select_gamma two) {
    int i  = 0, eq = 0;

    if(one.level != two.level)
        return 0;

    for(i = 0; i < one.level; ++i){
        if(one.fnames[i] == two.fnames[i]) eq++;
        else break;
    }

    return (eq == one.level);
}

/*----------------------------------------------------------*
 * erase an element from my hash                            *
 *----------------------------------------------------------*/

bool AOS_Hash::erase_element(aos_select_gamma *elmt) {
    int i = 0, j = 0, index = 0;
    double func = 0.0;

    //error check
    if(elmt == NULL) return 0;

    //hashing function
    for(i = 0; i < elmt->level; ++i)
        func += elmt->fnames[i] * elmt->fnames[i];

    index = (int)(fmod(func, length));

    //finding it
    i = 0;
    while(i < width && elements[index][i] != NULL){
        //check equality between nodes
        if(node_equal(*elmt, *elements[index][i])){
            //shift elements
            j = i+1;
            while(j < width && elements[index][j] != NULL){
                elements[index][j-1] = elements[index][j];
                j++;
            }
            //setting last element as null
            elements[index][j-1] = NULL;

            break;
        }
        i++;
    }
}

/*----------------------------------------------------------*
 * insert an element into my hash                           *
 *----------------------------------------------------------*/

bool AOS_Hash::add_element(aos_select_gamma *elmt) {
    int i = 0, index = 0;
    double func = 0;

    //error check
    if(elmt == NULL)
        return 0;

    //hashing function
    func = 0;
    for(i = 0; i < elmt->level; ++i)
        func += elmt->fnames[i] * elmt->fnames[i];

    index = (int)(fmod(func, length));

    //skiping equals
    i = 0;
    while(i < width && elements[index][i] != NULL){
        //check equality between nodes
        if(node_equal(*elmt, *elements[index][i])){
            //this node is identical to some other node
            //check if this node has real gamma
            if(elements[index][i]->rgamma < 0){
                //keep node with highest projected value
                if(elements[index][i]->value < elmt->value){
                    elements[index][i]->pgamma = elmt->pgamma;
                    elements[index][i]->value  = elmt->value;
                }
            }
            delete [] elmt->w;
            delete elmt;
            return 0;
        }

        i++;
    }

    //adding element
    if(i >= width){
        int filled = 0;
        for(i = 0; i < length; ++i)
            if(elements[i][0] == NULL) filled++;
        cout << "NEED RE-HASH! (just failed) (" << filled << "/" << length << " = " << ((double)filled/(length)*100.0) << "%" << endl;
        exit(1);
    }else{
        elements[index][i] = elmt;
        size++;
    }

    return 1;
}

/*----------------------------------------------------------*
 * print hash table                                         *
 *----------------------------------------------------------*/

void AOS_Hash::print(int dim) {
    int i = 0, j = 0, k = 0, d = 0;
    int cont = 0;

    for(i = 0; i < length; ++i)
        for(j = 0; j < width; ++j)
            if(elements[i][j] != NULL){
                cont++;
            }

    cout << "Cont = " << cont << endl;
    if(dim <= 10){
        for(d = 0; d < dim; ++d){
            for(i = 0; i < length; ++i)
                for(j = 0; j < width; ++j)
                    if(elements[i][j] != NULL)
                        if(elements[i][j]->level == d){
                            for(k = 0; k < (elements[i][j]->level)-1; ++k)
                                cout << elements[i][j]->fnames[k] << ", ";
                            cout << elements[i][j]->fnames[k] << endl;
                        }
            cout << endl;
        }
    }
}

int AOS_Hash::get_size() {
    return size;
}

AOS_Hash::~AOS_Hash() {
    int i = 0, j = 0;

    for(i = 0; i < length; ++i){
        for(j = 0; j < width; ++j){
            delete elements[i][j]->w;
        }
        delete elements[i][j];
    }
}