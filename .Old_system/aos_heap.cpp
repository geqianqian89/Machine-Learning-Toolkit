#include <iostream>
#include "aos_heap.h"

using namespace std;

/*----------------------------------------------------------*
 * creates a heap                                           *
 *----------------------------------------------------------*/

AOS_Heap::AOS_Heap() {
    elements.assign(MAX_HEAP+1, NULL);
    contheap = 0;
    contheapreins = 0;
    maxheapsize = 0;
    size = 0;
}

/*----------------------------------------------------------*
 * Insert into heap                                         *
 *----------------------------------------------------------*/

bool AOS_Heap::insert(aos_select_gamma *tok, int cont) {
    int i = 0;
    double val = 0;

    if(size == MAX_HEAP){
        if(tok->value > elements[MAX_HEAP]->value)
            i = MAX_HEAP;
        else return false;
    }else i = ++size;

    val = (elements[i/2] != NULL)? elements[i/2]->value : 0;

    while (i > 1 && val < tok->value){
        elements[i] = elements[i/2];
        i /= 2;
        val = (elements[i/2] != NULL) ? elements[i/2]->value : 0;
    }
    elements[i] = tok;

    if(size > maxheapsize)
        maxheapsize = size;

    if(cont) contheap++;
    else contheapreins++;
    return true;
}

/*----------------------------------------------------------*
 * Returns and remove the top element                                      *
 *----------------------------------------------------------*/

aos_select_gamma* AOS_Heap::pop(){
    aos_select_gamma *min_element = NULL;

    if(size == 0){
        cerr << "Tried to pop an empty heap!\n";
        return NULL;
    }

    min_element = elements[1];

    size--;
    percolate(1);

    return min_element;
}

/*----------------------------------------------------------*
 * Returns top element                                      *
 *----------------------------------------------------------*/

aos_select_gamma* AOS_Heap::top(){
    return elements[1];
}

/*----------------------------------------------------------*
 * percolates                                               *
 *----------------------------------------------------------*/

void AOS_Heap::percolate(int i) {
    int child = i * 2;
    aos_select_gamma *last_element = NULL;
    last_element = elements[size+1];

    if(child > size){
        elements[i] = last_element;
        return;
    }

    if(child != size && elements[child+1]->value > elements[child]->value)
        child++;

    //percolate one level
    if(last_element->value < elements[child]->value){
        elements[i] = elements[child];
        percolate(child);
    }
    else elements[i] = last_element;
}

/*----------------------------------------------------------*
 * Cont projected margin nodes in heap                      *
 *----------------------------------------------------------*/

int AOS_Heap::projected() {
    int i = 0, projected = 0;
    aos_select_gamma *curr = NULL;

    for(i = 1; i <= size; ++i){
        curr = elements[i];
        if(curr->rgamma < 0)
            projected++;
    }
    return projected;
}

/*----------------------------------------------------------*
 * removes old levels                                       *
 *----------------------------------------------------------*/

void AOS_Heap::cut(AOS_Hash &hash, int levelat, int cut, double gmargin, int verbose) {
    int i = 0, count = 0;
    aos_select_gamma *curr = NULL;

    for(i = size; i > 0; --i){
        curr = elements[i];
        if(curr->level < levelat){
            delete [] curr->w;
            delete curr;

            //percolate heap
            size--;
            percolate(i);
            count++;
        }
    }
    if(verbose) cout << "   [Nodes removed prooning: " << count << "]" << endl;

    countprooning += count;
}

/*----------------------------------------------------------*
 * Prints heap                                              *
 *----------------------------------------------------------*/

void AOS_Heap::print() {
    int i = 0, j = 0;
    vector<int> fnames;
    aos_select_gamma *curr = NULL;

    for(i = 1; i <= size; ++i){
        curr = elements[i];
        fnames = curr->fnames;
        cout << "Heap[" << i << "] --";
        if(curr->level <= 50){
            cout << " (";
            for(j = 0; j < curr->level-1; ++j)
                cout << fnames[j] << ", ";
            cout << fnames[j] << ")";
        }
        cout << " Value: " << curr->value << ", ";
        cout << " rMargin: " << curr->rgamma << ", ";
        cout << " pMargin: " << curr->pgamma << ", ";
        cout << " Level: " << curr->level << endl;
    }
}

int AOS_Heap::getContheapreins() const {
    return contheapreins;
}

int AOS_Heap::getCountprooning() const {
    return countprooning;
}

int AOS_Heap::getMaxheapsize() const {
    return maxheapsize;
}

int AOS_Heap::getContheap() const {
    return contheap;
}

int AOS_Heap::get_size() {
    return size;
}

const vector<aos_select_gamma *> &AOS_Heap::getElements() const {
    return elements;
}

/*----------------------------------------------------------*
 * Frees heap                                               *
 *----------------------------------------------------------*/

AOS_Heap::~AOS_Heap() {
    int i = 0;

    for(i = 1; i <= size; ++i){
        if(elements[i] != NULL){
            delete [] elements[i]->w;
        }
        delete elements[i];
    }
}





