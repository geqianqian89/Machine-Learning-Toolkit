//
// Created by mateus558 on 31/03/18.
//

#include <algorithm>
#include <cmath>
#include "../includes/RFE.hpp"

#define first_decay 0.25

using namespace std;

RFE::RFE(std::shared_ptr<Data> samples, Classifier *classifier, Validation::CrossValidation *cv, int depth, int skip, int jump, bool leave_one_out) {
    this->samples = samples;
    this->classifier = classifier;
    this->depth = depth;
    this->skip = skip;
    this->jump = jump;
    this->cv = cv;
    this->leave_one_out = leave_one_out;
}

std::unique_ptr<Data> RFE::selectFeatures() {
    int i = 0, j = 0;
    size_t dim = samples->getDim(), partial_dim = 0;
    vector<int> features(depth), partial_features, choosen_feats;
    vector<double> w, new_w;
    vector<select_weight> weight;
    int svcount = 0, level = 0, leveljump = 0, partial_svs = 0;
    int partial = 0; //verifica se última solução é uma solução recuperada (parcial)
    double max_time = classifier->getMax_time(), time_mult = samples->getTime_mult();
    double margin = 0, leave_oo = 0, kfolderror = 0, partial_time = 0, partial_margin = 0;
    double START_TIME = 100.0f * clock() / CLOCKS_PER_SEC;
    unique_ptr<Data> stmp_partial, stmp(make_unique<Data>());
    Validation validation(samples, classifier);

    *stmp = *samples;
    /*error check*/
    if (depth < 1 || depth >= dim) {
        cerr << "Invalid depth!\n";
        return 0;
    }

    /*inicializando o cross-validation*/
    if (cv->qtde > 0) {
        //utils_initialize_random();
        cv->seed.resize(cv->qtde);
        for (i = 0; i < cv->qtde; i++)
            cv->seed[i] = i; //rand();
        cv->initial_error = 0;
        cv->actual_error = 0;
    }
    double n0 = 1;

    Solution sol;

    classifier->setVerbose(0);
    validation.setVerbose(0);

    while (true) {
        svcount = 0;
        margin = 0;

        //if(level != 0) // || level == depth) //else stmp->max_time = max_time;
        if (level == 1)
            n0 = max_time *= first_decay;
        else if (level > 1)
            max_time = n0 * exp(-time_mult * ((double) dim / (dim - level)));

        classifier->setGamma(margin);
        classifier->setSolution(sol);

        /*training sample*/
        if (!classifier->train()) {

            sol = classifier->getSolution();
            svcount = sol.svs;
            margin = sol.margin;
            sol.w.erase(sol.w.begin(), sol.w.end());

            if (verbose) cerr << "Training Failed!\n";
            if (level > 0) {
                cout << "---------------\n :: FINAL :: \n---------------\n";
                choosen_feats = stmp_partial->getFeaturesNames();
                cout << "Choosen Features: ";
                for (i = 0; i < stmp_partial->getDim() - 1; ++i) cout << choosen_feats[i] << ", ";
                cout << choosen_feats[i] << endl;

                if (cv->qtde > 0) {
                    if ((dim - partial_dim) % cv->jump != 0) {
                        for (cv->actual_error = 0, i = 0; i < cv->qtde; i++) {
                            cv->actual_error += validation.kFold(cv->fold, cv->seed[i]);
                        }
                        kfolderror = cv->actual_error / cv->qtde;
                    }
                    cout << "Dim: " << partial_dim << ", Margin: " << partial_dim << ", SVs: " << partial_svs
                         << ", Error " << cv->fold << "-fold: " << kfolderror << "%\n";
                } else
                    cout << "Dim: " << partial_dim << ", Margin: " << partial_dim << ", SVs: " << partial_svs << endl;
                cout << "---------------\nTotal time: " << partial_time << "\n\n";
                partial = 1;
                //data_write(filename, stmp_parcial, 0);
                //free(weight);
            }
            break;
        }

        partial_margin = margin;
        partial_svs = svcount;
        partial_time = (100.0f*clock()/CLOCKS_PER_SEC-START_TIME)/100.0f;
        partial_dim = dim-level;

        stmp_partial.reset();
        stmp_partial = make_unique<Data>();
        *stmp_partial = *samples;

        partial_features.clear();
        if(level-jump > 0)
        {
            partial_features.resize((size_t)(level-jump));
        }
        for(i = 0; i < level-jump; ++i)
            partial_features[i] = features[i];

        if(cv->qtde > 0)
        {
            if(level == 0)
            {
                for (cv->initial_error = 0, i = 0; i < cv->qtde; i++) {
                    cv->initial_error += validation.kFold(cv->fold, cv->seed[i]);
                }
                kfolderror = cv->initial_error / cv->qtde;
            }
            else if(level % cv->jump == 0)
            {
                for (cv->actual_error = 0, i = 0; i < cv->qtde; i++) {
                    cv->actual_error += validation.kFold(cv->fold, cv->seed[i]);
                }
                kfolderror = cv->actual_error / cv->qtde;
            }
        }

        /*leave one out*/
        if(leave_one_out)
        {
         //   leave_oo = utils_leave_one_out(stmp, train, skip, 0);
            cout << "LeaveOO -- Dim: "<< (dim-level) << ", Margin: " << margin << ", LeaveOO: " << leave_oo << ", SVs: " << svcount << endl;
        }
        else if(verbose)
        {
            if(cv->qtde > 0 && level % cv->jump == 0)
                cout << "Dim: " << (dim - level) <<", Margin: " << margin << ", SVs: " << svcount <<", Erro " << cv->fold << "-fold: " << kfolderror << "%\n";
            else
                cout << "Dim: " << (dim-level) << ", Margem: " << margin << ", SVs: " << svcount << endl;
            //printf("Dim: %d, Margem: %lf, Distancia entre os centros: %f, SVs: %d\n", (dim-level), data_get_dist_centers(stmp), margin, svcount);
        }

        weight.resize(dim);
        auto fnames = samples->getFeaturesNames();

        for(i = 0; i < dim; ++i)
        {
            weight[i].w = w[i];
            weight[i].fname = fnames[i];
        }

        sort(weight.begin(), weight.end(), compare_weight_greater);

        cout << "---------------------\n";
        if(verbose > 1)
        {
            for(i = 0; i < dim; ++i)
                cout << weight[i].fname << ": " << weight[i].w << endl;
            cout << "---------------------\n";
        }

        /*stopping criterion*/
        if(level >= depth || (cv->qtde > 0 && cv->actual_error-cv->initial_error > cv->limit_error))
        {
            cout << "---------------\n :: FINAL :: \n---------------\n";
            //if(stmp->dim < 50)
            choosen_feats = samples->getFeaturesNames();
            cout << "Choosen Features: ";
            for(i = 0; i < dim-1; ++i) cout << choosen_feats[i] <<",";
            cout << choosen_feats[i] << endl;

            cout << "---------------\nEliminated Features: ";
            for(i = 0; i < leveljump-1; ++i) cout << features[i] << ",";
            cout << features[i] << endl;

            if(cv->qtde > 0)
            {
                if(level % cv->jump != 0)
                {
                    for (cv->actual_error = 0, i = 0; i < cv->qtde; i++) {
                        cv->actual_error += validation.kFold(cv->fold, cv->seed[i]);
                    }
                    kfolderror = cv->actual_error / cv->qtde;
                }
                cout << "Dim: " << (dim - level) <<", Margin: " << margin << ", SVs: " << svcount <<", Erro " << cv->fold << "-fold: " << kfolderror << "%\n";
            }
            else
                cout << "Dim: " << (dim-level) << ", Margem: " << margin << ", SVs: " << svcount << endl;

            cout << "---------------\nTotal time: " << (100.0f*clock()/CLOCKS_PER_SEC-START_TIME)/100.0f << "\n\n";

            //data_write(filename, stmp, 0);
            weight.clear();
            break;
        }

        if(level+jump > depth)
            leveljump = depth;
        else
            leveljump = level+jump;

        /*manutencao do w do pai para o IMA Primal*/
        if(classifier->classifierType() == "Primal")
        {
            for(j = 0; j < dim; ++j)
                for(i = level; i < leveljump; ++i)
                    if(weight[i-level].w == w[j])
                        w[j] = 0;

            new_w.resize(dim-leveljump);

            for(i = 0, j = 0; j < dim; ++j)
                if(w[j] != 0)
                    new_w[i++] = w[j];
            //novo_w[i] = w[j]; //bias nao copia mais
            w.clear();
            w = new_w;
        }
        else //IMA Dual e SMO
        {
            w.clear();
        }

        if(*stmp != *samples) stmp.reset();

        /*saving removed feature name*/
        for(i = level; i < leveljump; ++i)
        {
            cout << "Removing w = " << weight[i-level].w << "\n";
            features[i] = weight[i-level].fname;
        }
        cout << "---------------------\n";
        weight.clear();

        /*increment*/
        if(level+jump > depth)
        {
            level = depth;
            jump  = 0;
        }
        else
            level += jump;
        /*get temp data struct*/
        stmp->removeFeatures(features);
    }
    /*free stuff*/

    if(cv->qtde > 0) cv->seed.clear();

    //samples.reset();
    if(partial)
    {
        stmp.reset();
        return std::move(stmp_partial);
    }
    else
    {
        stmp_partial.reset();
        return std::move(stmp);
    }
}

/*----------------------------------------------------------*
 * Returns 1 for a > b, -1 a < b, 0 if a = b                *
 *----------------------------------------------------------*/
int RFE::compare_weight_greater(const select_weight &a, const select_weight &b) {
    /*                 V (greater)*/
    //printf("%d\n",(fabs(ia->w) > fabs(ib->w)) - (fabs(ia->w) < fabs(ib->w)));
    return (fabs(a.w) > fabs(b.w)) - (fabs(a.w) < fabs(b.w));
}
