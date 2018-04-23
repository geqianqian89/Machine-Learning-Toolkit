#ifndef AOS_HASH_H
#define AOS_HASH_H
#include <vector>

/* Gamma Struct: Used to stack margin values*/
struct aos_select_gamma{
    std::vector<int> fnames;
    int level;
    int sv;
    int train;
    double value; //value used as choice criterion
    double pgamma; //projected gamma
    double rgamma; //real gamma for display
    double radius; //radius
    double dcents; //distance between the centers
    double golub; //golub - statistics
    double fisher; //fisher - statistics
    double *w;
    double bias;
};

typedef std::vector<std::vector<aos_select_gamma*> > select_gamma_matrix;

class AOS_Hash{
private:
    int length, width, size;
    select_gamma_matrix elements;

    bool node_equal(aos_select_gamma one, aos_select_gamma two);
public:
    AOS_Hash(int length, int width);
    bool erase_element(aos_select_gamma *elmt);
    bool add_element(aos_select_gamma *elmt);
    void print(int dim);
    int get_size();
    ~AOS_Hash();
};

#endif //AOS_HASH_H
