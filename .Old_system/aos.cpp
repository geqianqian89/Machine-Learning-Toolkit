/*****************************************************
 * Admissible Ordered Search                         *
 *                                                   *
 * Saul Leite <lsaul@lncc.br>                        *
 * sep 23, 2004                                      *
 *                                                   *
 * Saulo Moraes Villela <saulomv@gmail.com>          *
 * 2009, 2011                                        *
 *****************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include "data.h"
#include "utils.h"
#include "kernel.h"
#include "aos.h"
#include "pl.h"
#include "smo.h"

#define HASH_SIZE 161387
#define HASH_WIDTH 100
#define MAX_BREATH 99999
#define MAX_DEPTH 99999
#define MAX_HEAP 500000
#define NUM_ERROR_EPS 0.05
#define first_decay 0.5

int contheap         = 0;
int contheapreins    = 0;
int conthash         = 0;
int contprooning     = 0;
int maxheapsize      = 0;
int contnotheap      = 0;
int conthashnotheap  = 0;
int contexpanded	 = 0;
int contprojected    = 0;
int contprojtrained  = 0;

double START_TIME, tempo_inicial, max_time_orig;

Sample *stmp_parcial = NULL;
double partial_time;
double partial_margin;
int partial_dim;
int partial_svs;

int partial_contheap;
int partial_contheapreins;
int partial_conthash;
int partial_contprooning;
int partial_maxheapsize;
int partial_contnotheap;
int partial_conthashnotheap;
int partial_contexpanded;
int partial_contprojected;
int partial_contprojtrained;
int partial_leftoverprojections;

double kfold_error = 0;

double n0 = 1;
int dim_orig = 1;


/*----------------------------------------------------------*
 * Run A* feature selection main loop                       *
 *----------------------------------------------------------*/
void aos_select_feature_main_loop(Sample *sample, AOS_Heap **heap,
                                  AOS_Hash **hash,
                                  int (*train)(Sample*,double**,double*,int*,int),
                                  int breadth, double bonus, int* lool, int startdim,
                                  int look_ahead_depth, double *g_margin, int cut, int skip,
                                  int startover, int doleave_oo, int depth, int sorting_type,
                                  int choice_shape, int ftime, crossvalidation *cv, int verbose){
    int i = 0, j = 0, k = 0;
    int level = 0, svcount = 0;
    vector<int> ofnames;
    vector<aos_select_weight> weight;
    double margin = 0.0, omargin = 0.0, wnorm = 0.0, tpmargin = 0.0, sumnorm = 0.0, leave_oo = -1;
    double *w = NULL, *w_manut = NULL; //w vector for look ahead maintenance
    aos_select_gamma *gtmp = NULL;
    AOS_Hash *hash = NULL;
    AOS_Heap *heap = *heapp;
    Data data;
    int dim = sample->dim, size = sample->size;
    double q = sample->q;
    bool loolflag = false; //close one dimension

    data.set_samples(sample);

    if(heap->get_size() == 0){
        if(ftime && sample->kernel_type == 9){  //First dimension -- exact solution
            if(sample->q == 2){
                sample->kernel_type = 0;
                if(!smo_train(sample, w, margin, svcount, 0)){
                    delete [] w;
                    if(verbose > 1) cerr << "Training failed!\n" << endl;
                    return;
                }
                sample->kernel_type = 9;
            }else if(sample->q == 1){
                if(!linear_programming(sample, &w, margin, svcount, 0)){
                    delete [] w;
                    if(verbose > 1) cerr << "Training failed!\n" << endl;
                    return;
                }
            }
        }else if(ftime && sample->mult_tempo == 2){ //First dimension -- dual solution - "gambiarra"
            if(!smo_train(sample, &w, &margin, &svcount, 0)){
                delete [] w;
                if(verbose > 1) cerr << "Training failed!\n" << endl;
                return;
            }
        }else{  //Not first dimension or not IMA
            //training
            if(!train(sample, &w, &margin, &svcount, 0)){
                delete [] w;
                if(verbose > 1) cerr << "Training failed!\n" << endl;
                return;
            }
        }
    }else{
       gtmp = heap->pop();

       if(gtmp->level > depth){
           gtmp = NULL;
           return;
       }

       w = gtmp->w;
       if(gtmp->rgamma > 0){
           margin = gtmp->rgamma;
           svcount = gtmp->sv;
       }else{
           //training
           margin = gtmp->pgamma;
           sample->bias = gtmp->bias;
           if(!train(sample, &w, &margin, &svcount, 0)){
               //training failed, remove this option
               delete [] w;
               gtmp = NULL;
               if(verbose > 1) cerr << "Training failed!\n" << endl;
               return;
           }

           if(choice_shape == 2)
               gtmp->value = margin * data.get_dist_centers(-1);
           else
               gtmp->value = margin;
           gtmp->rgamma = margin;
           gtmp->train  = 1;

           contprojtrained++;

           //verify if really is the best or if already got at the desired depth
           if(gtmp->value < heap->top()->value || gtmp->level == depth){
               gtmp->w  = w;
               gtmp->sv = svcount;
               heap->insert(gtmp, 0);
               return;
           }
       }
        //This is the best solution, continue
        contexpanded++;
        ofnames = gtmp->fnames;
        level = gtmp->level;
        omargin = gtmp->pgamma;
        gtmp = NULL;

        if(level > *lool){
            (*lool) = level;
            loolflag = true;    //Dimension closed
        }
    }

    if(verbose){
        if(level > 0){
            cout << "-> Expanding features (";
            for(i = 0; i < level - 1; ++i){
                cout << ofnames[i] << ", ";
            }
            cout << ofnames[i];
            cout << ") -- Margin: " << margin << ", pMargin: " << omargin << ", Level: " << level << endl;
        }
    }

    //calculating leave one out, if it's the first to hit this level
    if((*lool) == level && (loolflag == 1 || level == 0)){
        loolflag = 0;
        if(cv->qtd > 0){
            if(level == 0){
                for(cv->initial_error = 0, i = 0; i < cv->qtd; ++i){
                    cv->initial_error += utils_k_fold(sample, train, cv->fold, cv->seed[i], 1);
                }
                kfold_error = cv->initial_error / cv->qtd;
            }else if(level % cv->jump == 0){
                for(cv->actual_error = 0, i = 0; i < cv->qtd; ++i){
                    cv->actual_error += utils_k_fold(sample, train, cv->fold, cv->seed[i], 0);
                }
                kfold_error = cv->initial_error / cv->qtd;
            }
        }

        if(doleave_oo){
            leave_oo = utils_leave_one_out(sample, train, skip, 0);
            cout << "Leave One Out -- Dim: " << startdim-level << ", Margin: " << margin << ", LeaveOO: " << leave_oo << ", SVs: " << svcount << ", Time: " << ((100.0f*clock()/CLOCKS_PER_SEC)-tempo_inicial)/100.0f << endl;
        }else{
            leave_oo = -1;

            cout << "--- --- --- --- --- --- --- ---\n";
            if(cv->qtd > 0 && level % cv->jump == 0)
                cout << "Dim: " << startdim-level << ", Margin: " << margin << ", SVs: " << svcount << ", Erro " << cv->fold << "-fold: " << errokfold << ", Time: " << ((100.0f*clock()/CLOCKS_PER_SEC)-tempo_inicial)/100.0f << endl;
            else
                cout << "Dim: " << startdim-level << ", Margin: " << margin << ", SVs: " << svcount << ", Time: " << ((100.0f*clock()/CLOCKS_PER_SEC)-tempo_inicial)/100.0f << endl;
            if(level > 0){
                cout << "Eliminated features: ";
                for(j = 0; j < level - 1; ++j){
                    cout << ofnames[j] << ", ";
                }
                cout << ofnames[j] << endl;
            }
            cout << "Ins: " << contheap << " / ";
            cout << "ReIns: " << contheapreins << " / ";
            cout << "Max: " << " / " << maxheapsize << " / ";
            cout << "Podas: " << contprooning << " / ";
            cout << "Hash: " << conthash << " / ";
            cout << "Expanded: " << contexpanded << " / ";
            cout << "Not Trained: " << contprojected-contprojtrained;
            cout << "\n--- --- --- --- --- --- --- ---\n";
        }

        //saving info from last closed dimension
        partial_margin = margin;
        partial_svs = svcount;
        partial_time = ((100.0f*clock()/CLOCKS_PER_SEC)-tempo_inicial)/100.0f;
        partial_dim = startdim-level;
        partial_contheap = heap->getContheap();
        partial_contheapreins = heap->getContheapreins();
        partial_conthash = conthash;
        partial_contprooning = heap->getCountprooning();
        partial_maxheapsize = heap->getMaxheapsize();
        partial_contnotheap = contnotheap;
        partial_conthashnotheap = conthashnotheap;
        partial_contexpanded = contexpanded;
        partial_contprojected = contprojected;
        partial_contprojtrained = contprojtrained;

        delete stmp_parcial;
        stmp_parcial = Sample::copy(sample);

        if(look_ahead_depth > 0){
            if(sample->kernel_type == 9){
                //create a maintenance w for the
                w_manut = new double[dim];
                /*if(w_manut == NULL){
                    cout << "Error: Out of memory 1" << endl;
                    exit(1);
                }*/
                memcpy(w, w_manut, dim*sizeof(double));
                delete w_manut;
            }
        }
        /*cut heap based on its level and look ahead margin*/
        heap->cut(hash, *lool, cut, *g_margin, verbose);

        if(verbose > 2){
            cout << " (" << heap->get_size() << "\t";
            for(j = 0; j < level; ++j){
                cout << ofnames[j] << " ";
            }
            cout << endl;
        }
    }

    //allocating space for w/feature
    weight.resize(dim);

    //copying elements of array
    for(i = 0; i < dim; ++i){
        weight[i].w = w[i];
        weight[i].index = i;
        weight[i].fname = sample->fnames[i];
        weight[i].radius = -1;
        weight[i].dcents = -1;
        weight[i].golub = -1;
        weight[i].fisher = -1;
    }

    if(sorting_type == 2){
        for(i = 0; i < dim; ++i){
            weight[i].dcents = data.get_dist_centers(weight[i].fname);
        }
    }else if(sorting_type == 3){
        for(i = 0; i < dim; ++i){
            weight[i].radius = data.get_radius(weight[i].fname, q);
            weight[i].dcents = data.get_dist_centers(weight[i].fname);
        }
    }else if(sorting_type == 4){
        for(i = 0; i < dim; ++i){
            weight[i].radius = data.get_radius(weight[i].fname, q);
        }
    }else if(sorting_type == 5 || sorting_type == 6) {
        int num_pos = 0, num_neg = 0;

        vector<double> avg_pos(dim), avg_neg(dim), sd_pos(dim), sd_neg(dim);

        //calc average
        for (i = 0; i < dim; ++i) {
            num_neg = 0;
            num_pos = 0;
            avg_neg[i] = 0;
            avg_pos[i] = 0;

            for (j = 0; j < size; ++j) {
                if (sample->points[j].y == -1) {
                    avg_neg[i] += sample->points[j].x[i];
                    num_neg++;
                } else {
                    avg_pos[i] += sample->points[j].x[i];
                    num_pos++;
                }
            }
            avg_neg[i] /= num_neg;
            avg_pos[i] /= num_pos;
        }

        //calc variance
        for (i = 0; i < dim; ++i) {
            sd_neg[i] = 0;
            sd_pos[i] = 0;
            for (j = 0; j < size; ++j) {
                if (sample->points[j].y == -1) {
                    sd_neg[i] += pow(sample->points[j].x[i] - avg_neg, 2);
                } else {
                    sd_pos[i] += pow(sample->points[j].x[i] - avg_pos[i], 2);
                }
            }
        }

        for (i = 0; i < dim; ++i) {
            weight[i].golub = fabs(avg_pos[i] - avg_neg[i]) / (sqrt(sd_pos[i] / (num_pos - 1)) + sqrt(sd_neg[i] / (num_neg - 1)));
            weight[i].fisher = pow(avg_pos[i] - avg_neg[i], 2) / (sd_pos[i] + sd_neg[i]);
        }
    }

    //sorting type
    if(sorting_type == 6){
        sort(weight.begin(), weight.end(), aos_select_compare_weightfisher_greater);
    }else if(sorting_type == 5){
        sort(weight.begin(), weight.end(), aos_select_compare_weightgolub_greater);
    }else if(sorting_type == 4){
        sort(weight.begin(), weight.end(), aos_select_compare_weightradius_greater);
    }else if(sorting_type == 3){
        sort(weight.begin(), weight.end(), aos_select_compare_weightradiuscenter_greater);
    }else if(sorting_type == 2){
        sort(weight.begin(), weight.end(), aos_select_compare_weightcenter_greater);
    }else{
        sort(weight.begin(), weight.end(), aos_select_compare_weight_greater);
    }

    //getting norm
    if(sample->kernel_type == 9){
        if(q == 1){
            for(wnorm = 0, i = 0; i < dim; ++i){
                wnorm += fabs(w[i]);
            }
        }else{
            vector<double> w_v(dim);
            for(i = 0; i < dim; ++i){w_v[i] = w[i];}
            wnorm = utils_norm(w_v, q);
        }
    }else{
        double **matrix = kernel_generate_matrix(sample);
        wnorm = kernel_feature_space_norm(sample, matrix);
        kernel_free_matrix(matrix, size);
    }

    //ramificando nodos
    for(i = 0; i < breadth; ++i){
        if(sample->kernel_type == 9){
            //computation of projected margin on IMA Primal
            if(dim > 1){
                if(q == 1){
                    for(j = 0, sumnorm = 0; j < dim; ++j){
                        if(i != j){
                            sumnorm += fabs(weight[j].w);
                        }
                    }
                }else{
                    for(sumnorm = 0, j = 0; j < dim; ++j){
                        if(i != j){
                            sumnorm += pow(fabs(weight[j].w, q));
                        }
                    }
                    sumnorm = pow(sumnorm, 1.0/q);
                }
                tpmargin = sumnorm/wnorm*margin;
            }else{
                sumnorm = pow(fabs(weight[0].w), q);
                sumnorm = pow(sumnorm, 1.0/q);
                tpmargin = sumnorm/wnorm*margin;
            }
        }else{
            //computation of the projectedd margin on IMA Dual and SMO
            double **Hk = kernel_generate_matrix_H_without_dim(sample, weight[i].index);
            vector<double> alphaaux(size);

            for(k = 0; k < size; ++k){
                for(auphaaux[k] = 0, j = 0; j < size; ++j){
                    alphaaux[k] += sample->points[j].alpha * Hk[k][j];
                }
            }

            for(sumnorm = 0, k = 0; k < size; ++k){
                sumnorm += alphaaux[k] * sample->points[k]->alpha;
            }
            sumnorm = sqrt(sumnorm);
            tpmargin = sumnorm/wnorm*margin;

            kernel_free_matrix(Hk, size);
        }

        //creating new nodes for heap
        gtmp = new aos_select_gamma;

        if(choice_shape == 2){
            if(weight[i].w == 0){
                gtmp->value = margin * weight[i].dcents;
            }else{
                gtmp->value = tpmargin*weight[i].dcents;
            }
        }else{
            if(weight[i].w == 0){
                gtmp->value = margin;
            }else{
                gtmp->value = tpmargin;
            }
        }
        gtmp->pgamma = tpmargin;
        if(weight[i].w == 0){
            gtmp->rgamma = margin;
            gtmp->train = 1;
        }else{
            gtmp->rgamma = -1;
            gtmp->train = 0;
        }

        gtmp->w = NULL;
        gtmp->bias = 0;
        gtmp->sv = 0;
        gtmp->level = level+1;
        gtmp->radius = weight[i].radius;
        gtmp->dcents = weight[i].dcents;
        gtmp->golub = weight[i].golub;
        gtmp->fisher = weight[i].fisher;

        //maintenance of the W from the father
        if(sample->kernel_type == 9){
            gtmp->w = new double[dim];

            for(k = 0, j = 0; j < dim; ++j){
                if(sample->fnames[j] != weight[i].fname)
                    gtmp->w[k++] = w[j];
                gtmp->bias = sample->bias;
            }
        }

        //creating new fname array
        gtmp->fnames.resize(level+1);

        for(j = 0; j < level; ++j){
            gtmp->fnames[j] = ofnames[j];
        }
        gtmp->fnames[j] = weight[i].fname;

        //sorting new feature array
        sort(gtmp->fnames.begin(), gtmp->fnames.end(), aos_select_compare_int_greater);

        if(choice_shape == 2){
            if(i != 0 && tpmargin*weight[i].dcents < (1 - NUM_ERROR_EPS) * (*g_margin)){
                hash->add_element(gtmp);
                contnotheap++;
                continue;
            }
        }else{
            if(i != 0 && tpmargin < (1 - NUM_ERROR_EPS)*(*g_margin)){
                hash->add_element(gtmp);
                contnotheap++;
                continue;
            }
        }

        //some verbose
        if(verbose){
            if(sorting_type == 5 || sorting_type == 6)
                cout << "   -- New node - Feature: " << weight[i].fname << ", Value: " << gtmp->value << ", pMargin: " << gtmp->pgamma << ", DCent: " << gtmp->dcents << ", Radius: " << gtmp->radius << ", Golub: " << gtmp->golub << ", Fisher: " << gtmp->fisher << ", Level: " << gtmp->level << endl;
            else
                cout << "   -- New node - Feature: " << weight[i].fname << ", Value: " << gtmp->value << ", rMargin: " << gtmp->rgamma << ", pMargin: " << gtmp->pgamma << ", DCent: " << gtmp->dcents << ", Radius: " << gtmp->radius << ", Level: " << gtmp->level << endl;
        }

        //push node into heap if it is not redundant in hash
        if(hash->add_element(gtmp)){
            heap->insert(gtmp,1);
            contprojected++;
        }else{
            conthashnotheap++;
        }
    }
}

/*-----------------------------------------------------------*
 * Run A* feature selection                                  *
 *                                                           *
 * breadth = maximum search breadth.                         *
 * depth   = stop when depth is reached                      *
 * bonus   = bonus value in formula margin*(1+bonus(level+1))*
 * look_ahead_depth = depth of puring value search           *
 *-----------------------------------------------------------*/

Sample* aos_select_features(string filename, Sample *sample,
                            int (*train)(struct sample*,double**,double*,int*,int),
                            int breadth, int depth, double bonus, int cut,
                            int look_ahead_depth, int skip, int startover,
                            int doleave_oo, int sorting_shape, int choice_shape,
                            crossvalidation *cv, int verbose)
{
    int i = 0, tbreadth = 0, level = 0, dim = sample->dim, startdim = dim_orig = dim;
    int ftime = 1, lool = 0;    /*leave one out level*/
    vector<int> fnames;
    aos_select_gamma *heap_top;
    double g_margin = 0;
    struct sample* stmp   = NULL;
    AOS_Heap *heap = NULL;
    AOS_Hash *hash = NULL;
    max_time_orig = sample->max_time;
    Data data;

    int parcial = 0; //verify if the last solution is a recovered solution (partial)

    /*counters initialization*/
    contheap         = 0;
    contheapreins    = 0;
    conthash         = 0;
    contprooning     = 0;
    maxheapsize      = 0;
    contnotheap      = 0;
    conthashnotheap  = 0;
    contexpanded     = 0;
    contprojected    = 0;
    contprojtrained  = 0;

    START_TIME = 100.0f*clock()/CLOCKS_PER_SEC;;
    tempo_inicial = START_TIME;

    //Checking arguments
    if(breadth > MAX_BREATH){ breadth = MAX_BREATH; }
    if(depth > dim - 1) { depth = dim - 1; }
    if(depth > MAX_DEPTH) { depth = MAX_DEPTH; }

    hash = new AOS_Hash(HASH_SIZE, HASH_WIDTH);
    heap = new AOS_Heap;

    //initializing cross-validation
    if(cv->qtd > 0){
        cv->seed = new int[cv->qtd];
        for(i = 0; i < cv->qtd; i++){
            cv->seed[i] = i;
        }
        cv->initial_error = 0;
        cv->actual_error = 0;
    }

    //do this while my depth permits
    while(1){
        //First problem to solve, when heap is empty
        if(heap->get_size() == 0){
            //quit end of heap found
            if(ftime == 0){ //first time = false -- it's not the first time, proccess failed!
                cout << "End of the Heap, recovering last dimension...\n\n";

                //get data from last lool, once that was the last dimension closed
                cout << "---------------\n :: END :: \n---------------\n";
                cout << "Choosen Features: ";
                for(i = 0; i < stmp_parcial->dim-1; ++i){
                    cout << stmp_parcial->fnames[i] << ", ";
                }
                cout << stmp_parcial->fnames[i] << endl;

                if(cv->qtd > 0){
                    if(level % cv->jump != 0){
                        for(cv->actual_error = 0, i = 0; i < cv->qtd; i++){
                            cv->actual_error += utils_k_fold(stmp_parcial, train, cv->fold, cv->seed[i], 0);
                        }
                        kfold_error = cv->actual_error / cv->qtd;
                    }
                    cout << "Dim: " << partial_dim << ", Margin: " << partial_margin << ", SVs: " << partial_svs << ", " << "Error " << cv->fold << "-fold: " << kfold_error << endl;
                }else
                    cout << "Dim: " << partial_dim << ", Margin: " << partial_margin << ", SVs: " << partial_svs << endl;
                cout << "Total of insertions in Heap: " << partial_contheap << endl;
                cout << "Total of reinsertions in Heap: " << partial_contheapreins << endl;
                cout << "Max size of the Heap: " << partial_maxheapsize << endl;
                cout << "Total prooning in Heap: " << partial_contprooning << endl;
                cout << "Expanded nodes: " << partial_contexpanded << endl;
                cout << "Not inserted in heap: " << partial_contnotheap << endl;
                cout << "Total of projections: " << partial_contprojected << endl;
                cout << "Total of trained projections: " << partial_contprojtrained << endl;
                cout << "Total of projections not trained: " << partial_contprojected << endl;
                cout << "Equal nodes on Hash that not entered in Heap: " << partial_contprojected-partial_contprojtrained << endl;
                cout << "Hash size: " << partial_conthash << endl;
                cout << "Total time: " << partial_time << endl;
                partial = 1;
                //save data to file
                data.write(filename, stmp_parcial, 0);
                break;
            }

            //check breadth
            tbreadth = breadth;
            if(tbreadth > dim) tbreadth = dim;

            //run select
            aos_select_feature_main_loop(sample, &heap, &hash, train,
                    tbreadth, bonus, &lool, startdim, look_ahead_depth,
                    &g_margin, cut, skip, startover, doleave_oo, depth,
                    sorting_shape, choice_shape, ftime, cv, verbose);
            if(heap->get_size() == 0){
                cout << "Initial training failed!!!\n\n";
                break;
            }
            ftime = 0;
        }else{  //subsequent problems (heap not empty)
            //create new data struct
            level = heap->getElements()[1]->level;
            fnames = heap->getElements()[1]->fnames;
            stmp = data.remove_features(sample, fnames, level, 0);

            if(level == 1)
                n0 = stmp->max_time *= first_decay;
            else if(level > 1)
                stmp->max_time = n0 * exp(-stmp->mult_tempo * ((double)startdim/(startdim-level)));

            heap_top = heap->top();
            //stop criterium
            if(stmp->dim == dim - depth && heap_top->rgamma > 0){
                cout << "---------------\n :: END :: \n---------------\n";
                cout << "Choosen Features: ";
                for(i = 0; i < stmp_parcial->dim-1; ++i){
                    cout << stmp_parcial->fnames[i] << ", ";
                }
                cout << stmp_parcial->fnames[i] << endl;

                cout << "---------------\nEliminated Features: ";
                for(i = 0; i < sample->dim-stmp->dim-1; ++i){
                    cout << heap_top->fnames[i];
                }
                cout << heap_top->fnames[i] << endl;

                if(cv->qtd > 0){
                    if(level % cv->jump != 0){
                        for(cv->actual_error = 0, i = 0; i < cv->qtd; i++){
                            cv->actual_error += utils_k_fold(stmp_parcial, train, cv->fold, cv->seed[i], 0);
                        }
                        kfold_error = cv->actual_error / cv->qtd;
                    }
                    cout << "Dim: " << stmp->dim << ", Margin: " << heap_top->rgamma << ", SVs: " << heap_top->sv << ", " << "Error " << cv->fold << "-fold: " << kfold_error << endl;
                }else
                    cout << "Dim: " << stmp->dim << ", Margin: " << heap_top->rgamma << ", SVs: " << heap_top->sv << endl;
                cout << "Total of insertions in Heap: " << contheap << endl;
                cout << "Total of reinsertions in Heap: " << contheapreins << endl;
                cout << "Max size of the Heap: " << heap->getMaxheapsize() << endl;
                cout << "Total prooning in Heap: " << contprooning << endl;
                cout << "Expanded nodes: " << partial_contexpanded << endl;
                cout << "Not inserted in heap: " << partial_contnotheap << endl;
                cout << "Total of projections: " << partial_contprojected << endl;
                cout << "Total of trained projections: " << partial_contprojtrained << endl;
                cout << "Total of projections not trained: " << partial_contprojected << endl;
                cout << "Equal nodes on Hash that not entered in Heap: " << partial_contprojected-partial_contprojtrained << endl;
                cout << "Hash size: " << partial_conthash << endl;
                cout << "Total time: " << ((100.0f*clock()/CLOCKS_PER_SEC)-tempo_inicial)/100.0f << endl;

                //save data to file
                data.write(filename, stmp, 0);
                break;
            }

            //some verbose
            if(verbose && level <= 50){
                cout << "-- Testing the node - Feature (";
                for(i = 0; i < level-1; ++i)
                    cout << fnames[i];
                cout << fnames[i] << endl;
                cout << "------------------------------------------\n";
            }

            /*check breadth*/
            tbreadth = breadth;
            if(tbreadth > stmp->dim) tbreadth = stmp->dim;

            /*run select*/
            aos_select_feature_main_loop(stmp, &heap, &hash, train, tbreadth, bonus,
                    &lool, startdim, look_ahead_depth, &g_margin, cut, skip, startover,
                    doleave_oo, depth, sorting_shape, choice_shape, ftime, cv, verbose);

            /*free stuff*/
            delete stmp;
        }

        /*verbose*/
        if(verbose){
            if(verbose > 1){
                heap->print();
                fflush(stdout);
            }else
                cout << "-- Heap Size: " << heap->get_size() << endl;
            cout << "--------\n" << endl;
        }
    }

    /*free stuff*/
    delete hash;
    delete heap;
    delete sample;
    if(cv->qtd > 0) delete [] cv->seed;
    if(partial){
        delete stmp;
        return stmp_parcial;
    }else{
        delete stmp_parcial;
        return stmp;
    }
}

/*----------------------------------------------------------*
 * look ahead for pruning value                             *
 *----------------------------------------------------------*/
double aos_select_look_ahead(Sample *sample, AOS_Heap *heap, AOS_Heap *hash, vector<int> &fnames_orig, double *w_orig,
                             int (*train)(Sample*,double**,double*,int*,int),
                             int depth, int level_orig, int look_ahead_depth,
                             double bonus, int choice_shape, int verbose)
{
    int i = 0, j = 0;
    int level = level_orig;
    int svcount = 0;
    int count = 0;
    int feat = 0;
    vector<int> features(look_ahead_depth+1);
    double min = 0;
    double margin = 0;
    double g_margin = 0;
    double *w = w_orig;
    double *new_w = NULL;
    struct sample *stmp = sample;
    aos_select_gamma *gtmp = NULL;
    double distcents = 0;
    Data data;

    data.set_samples(stmp);

    while(1){
        /*stopping criterion*/
        if(count == look_ahead_depth || count == sample->dim-1 || stmp->dim == sample->dim-depth || level == depth)
            break;

        if(choice_shape == 2){
            /*selecting one feature with least w / dist. centers*/
            min = fabs(w[0])/data.get_dist_centers(stmp->fnames[0]);
            feat = stmp->fnames[0];
            for(i = 1; i < stmp->dim; i++){
                distcents = data.get_dist_centers(stmp->fnames[i]);
                if(fabs(w[i])/distcents < min){
                    min = fabs(w[i]) / distcents;
                    feat = stmp->fnames[i];
                }
            }
        }else{
            /*Selecting one feature with least w */
            min = fabs(w[0]);
            feat = stmp->fnames[0];
            for(i = 1; i < stmp->>dim; i++)
                if(fabs(w[i]) < min){
                    min = fabs(w[i]);
                    feat = stmp->fnames[i];
                }
        }

        /*maitenance of the w fathe for the IMA Primal*/
        if(stmp->kernel_type == 9){
            new_w = new double[stmp->dim - 1];
            for(i = 0, j = 0; j < stmp->dim; ++j)
                novo_w[i++] = w[j];
        }

        /*saving removed feature name*/
        features[count] = feat;

        /*removing old data sample*/
        if(stmp != sample) delete stmp;

        /*get temp data struct*/
        stmp = data.remove_features(sample, features, (count+1), 0);

        if(level == 0)
            n0 = stmp->max_time = max_time_orig * first_decay;
        else
            stmp->max_time = n0 * exp(-stmp->mult_tempo * ((double)dim_orig/(dim_orig-level-1)));

        /*creating new nodes for heap*/
        gtmp = new aos_select_gamma;

        /*setting values*/
        gtmp->value  = -1;
        gtmp->pgamma = stmp->margin;
        gtmp->rgamma = -1;
        gtmp->train  =  0;
        gtmp->sv     =  0;
        gtmp->w      = NULL;
        gtmp->bias   = 0;
        gtmp->level  = level+1;
        gtmp->radius   = -1;
        if(choice_shape == 2)
            gtmp->dcents = data.get_dist_centers(-1);
        else
            gtmp->dcents = -1;
        gtmp->fisher = -1;
        gtmp->golub = -1;

        /*creating new fnames array*/
        gtmp->fnames.resize(level+1);

        for(j = 0; j < level_orig; ++j) gtmp->fnames[j] = fnames_orig[j];
        for(j = 0; j < count+1; ++j)    gtmp->fnames[level_orig+j] = features[j];

        /*sorting new feature array*/
        sort(gtmp->fnames.begin()+level+1, gtmp->fnames.end(), aos_select_compare_int_greater);

        if(verbose){
            cout << "   -- New look-ahead node - Features: ";
            for(i = 0; i < count; i++)
                cout << features[i] << ", ";
            cout << features[i] << endl;
        }

        /*push node into heap if it is not redundant in hash*/
        if(hash->add_element(gtmp)){
            /*training sample*/
            svcount = 0;
            margin = gtmp->pgamma;
            stmp->bias = gtmp->bias;
            if(!train(stmp, &new_w, &margin, &svcount, 0)){
                delete new_w;
                if(verbose) cout << "Training failed!" << endl;
                 break;
            }
            gtmp->w = new_w;
            gtmp->bias = stmp->bias;
            gtmp->sv = svcount;
            gtmp->rgamma = margin;
            gtmp->train = 1;
            if(choice_shape == 2)
                gtmp->value = margin * gtmp->dcents;
            else
                gtmp->value = margin;
            g_margin = gtmp->value;
            heap->insert(gtmp, 1);
        }
        if(stmp->kernel_type == 9){
            w = new double[dim];
            memcpy(w, new_w, (stmp->dim)*sizeof(double));
        }

        /*increment*/
        level++;
        count++;
    }

    w_orig = w;

    /*free stuff*/
    delete [] features;
    if(stmp != sample)  delete stmp;

    /*return gamma*/
    return g_margin;
}

/***********************************************************
 *              OTHER FUNCTIONS                            *
 ***********************************************************/

/*----------------------------------------------------------*
 * checks if two nodes are the same                         *
 *----------------------------------------------------------*/

bool aos_select_node_equal(aos_select_gamma one, aos_select_gamma two){
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
 * Returns 1 for a > b, -1 a < b, 0 if a = b                *
 *----------------------------------------------------------*/

bool aos_select_compare_int_greater(int a, int b){
	return a > b;
}

bool aos_select_compare_weight_greater(aos_select_weight a, aos_select_weight b){
	return fabs(a->w) > fabs(b->w);
}

bool aos_select_compare_weightradius_greater(aos_select_weight a, aos_select_weight b){
	return (fabs(a.w * a.radius) > fabs(b.w * b.radius));
}

bool aos_select_compare_weightcenter_greater(aos_select_weight a, aos_select_weight b){
	return (fabs(a.w/a.dcents) > fabs(b.w/b.dcents));
}

bool aos_select_compare_weightradiuscenter_greater(aos_select_weight a, aos_select_weight b){
	return fabs(a.w * a.radius/a.dcents) > fabs(b.w * b.raio/b.dcents);
}

bool aos_select_compare_weightfisher_greater(aos_select_weight a, aos_select_weight b){
	return (fabs(a.w * a.fisher) > fabs(b.w * b.fisher));
}

bool aos_select_compare_weightgolub_greater(aos_select_weight a, aos_select_weight b){
	return (fabs(a.w * a.golub) > fabs(b.w * b.golub));
}
