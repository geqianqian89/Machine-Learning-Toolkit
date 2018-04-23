#ifndef AOS_HEAP_H
#define AOS_HEAP_H

#include "aos_hash.h"

#define MAX_HEAP 500000

class AOS_Heap {
private:
    int size, maxheapsize, contheap, contheapreins, countprooning;
    std::vector<aos_select_gamma*> elements;
public:
    const std::vector<aos_select_gamma *> &getElements() const;

public:
    AOS_Heap();
    bool insert(aos_select_gamma* tok, int cont);
    aos_select_gamma* pop();
    aos_select_gamma* top();
    void print();
    int projected();
    void percolate(int i);
    void cut(AOS_Hash &hash, int levelat, int cut, double gmargin, int verbose);
    int get_size();
    int getMaxheapsize() const;
    int getContheap() const;
    int getContheapreins() const;
    int getCountprooning() const;
    ~AOS_Heap();
};

#endif //AOS_HEAP_H
