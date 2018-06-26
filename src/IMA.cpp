//
// Created by mateus558 on 01/08/17.
//

#include <iostream>
#include <cmath>
#include <ctime>
#include <float.h>
#include "../includes/IMA.hpp"
#include "../includes/Timer.hpp"
#include "../includes/Perceptron.hpp"

using namespace std;

template < typename T >
IMAp< T >::IMAp(std::shared_ptr<Data< T > > samples, double margin,  Solution *initial_solution) {
    this->samples = samples;
    this->margin = margin;

    this->hasInitialSolution = false;

    if(initial_solution){
        this->w = initial_solution->w;
        this->solution.bias = initial_solution->bias;
        this->hasInitialSolution = true;
    }else{
        this->w.resize(samples->getDim());
    }
}

template < typename T >
bool IMAp< T >::train() {
    unsigned int tMax = 0;
    int i, j, n, maiorn = 0, flagNao1aDim = 0, y, it, sign = 1, svs = 0;
    size_t size = this->samples->getSize(), dim = this->samples->getDim(), t1=1, t3=1;
    double gamma, secs, bias = 0.0, alpha, rmargin = margin, inc, stime;
    double min = 0.0, max = 0.0, norm = 1.0, maiorw = 0.0;
    vector<double> w_saved, func;
    vector<int> index = this->samples->getIndex(), fnames = this->samples->getFeaturesNames();
    auto points = this->samples->getPoints();
    IMApFixedMargin< T > imapFixMargin(this->samples, gamma);
    Solution tempSol;

    this->timer.Reset();

    n = dim;
    this->rate = 1.0;
    //Initializing data struct
    this->solution.norm  = 1.0;

    //Allocating space for w_saved and func
    w_saved.resize(dim);
    func.resize(size);

    //Allocating space for w
    if(this->hasInitialSolution) {
        if (this->q == 1)
            for (this->solution.norm = 0.0, i = 0; i < dim; ++i) this->solution.norm += fabs(this->w[i]);
        else if (this->q == 2) {
            for (this->solution.norm = 0.0, i = 0; i < dim; ++i) this->solution.norm += this->w[i] * this->w[i];
            this->solution.norm = sqrt(this->solution.norm);
        } else {
            for (this->solution.norm = 0.0, i = 0; i < dim; ++i) this->solution.norm += pow(fabs(this->w[i]), this->q);
            this->solution.norm = pow(this->solution.norm, 1.0 / this->q);
        }

        for (i = 0; i < dim; ++i) this->w[i] /= this->solution.norm;

        this->solution.bias /= this->solution.norm;
        this->solution.norm = 1;
        flagNao1aDim = 1;
        int flag = 0;

        for (min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i) {
            y = points[i]->y;
            for (func[i] = 0, j = 0; j < dim; ++j)
                func[i] += this->w[j] * points[i]->x[j];
            if (y == 1 && func[i] < min) min = func[i];
            else if (y == -1 && func[i] > max) max = func[i];
        }

        this->solution.bias = -(min + max) / 2.0;

        for (min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i) {
            y = points[i]->y;
            for (func[i] = this->solution.bias, j = 0; j < dim; ++j)
                func[i] += this->w[j] * points[i]->x[j];
            if (func[i] * y < 0) flag++;
            if (y == 1 && func[i] < min) min = func[i];
            else if (y == -1 && func[i] > max) max = func[i];
        }

        if (flag) rmargin = 0;
        else rmargin = fabs(min);

        if (margin == 0) tMax = this->MAX_UP;
        else {
            double raio = Statistics< T >::getRadius(*this->samples, -1, this->q);//data_get_radius(sample, -1, this->q);
            tMax = (raio * raio - rmargin * rmargin) / pow(this->margin - rmargin, 2);
            if (rmargin == 0) tMax *= 1.5;
        }

        *imapFixMargin.gettMax() = tMax;
    }

    //Allocating space for index and initializing
    if(index.size() == 0)
    {
        index.resize(size);
        for(i = 0; i < size; ++i) index[i] = i;
    }
    this->samples->setIndex(index);

    //Initializing alpha
    for(i = 0; i < size; ++i) { points[i]->alpha = 0.0; }

    if(this->verbose)
    {
        cout << "-----------------------------------------------------------------------------\n";
        cout << " pmf    steps     updates              margin              norm          secs\n";
        cout << "-----------------------------------------------------------------------------\n";
    }

    it = 0; this->ctot = 0; this->steps = 0; gamma = 0.0;

    imapFixMargin.setCtot(this->ctot);
    imapFixMargin.setqNorm(this->q);
    imapFixMargin.setSteps(this->steps);
    imapFixMargin.setGamma(gamma);
    imapFixMargin.setFlexible(this->flexible);
    imapFixMargin.setLearningRate(this->rate);
    imapFixMargin.setMaxTime(this->max_time);
    stime = this->timer.Elapsed();
    imapFixMargin.setStartTime(stime);
    *imapFixMargin.getFlagNot1aDim() = flagNao1aDim;

    while(imapFixMargin.train())
    {
        stime += imapFixMargin.getElapsedTime();
        //imapFixMargin.setStartTime(stime);

        this->ctot = imapFixMargin.getCtot();
        this->steps = imapFixMargin.getSteps();
        //Finding minimum and maximum functional values
        tempSol = imapFixMargin.getSolution();
        norm  = tempSol.norm;
        bias  = tempSol.bias;
        func = tempSol.func;

        for(svs = 0, min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i)
        {
            y = points[i]->y;
            alpha = points[i]->alpha;
            if((func[i] + y*alpha*this->flexible) >= 0 && min > (func[i] + y*alpha*this->flexible)/norm){ min = (func[i] + y*alpha*this->flexible)/norm; }
            else if((func[i] + y*alpha*this->flexible) <  0 && max < (func[i] + y*alpha*this->flexible)/norm) max = (func[i] + y*alpha*this->flexible)/norm;
        }
        //Saving good weights
        for(i = 0; i < dim; i++) w_saved[i] = tempSol.w[i];

        //Obtaining real margin
        rmargin = (fabs(min) > fabs(max)) ? fabs(max) : fabs(min);

        //Shift no bias
        double mmargin = (fabs(max) + fabs(min)) / 2.0;
        if(fabs(max) > fabs(min))
            tempSol.bias += fabs(mmargin - rmargin);
        else
            tempSol.bias -= fabs(mmargin - rmargin);

        //Obtaining new gamma_f
        gamma = (min-max)/2.0;
        inc = (1+this->alpha_aprox)*rmargin;
        if(gamma < inc) gamma = inc;
        rmargin = mmargin;

        if(it == 2)
            t1 = this->ctot;
        t3 = this->ctot;

        if(it > 1)
        {
            this->rate = sqrt(t1) / sqrt(t3);
            if(this->verbose) cout << "RATE: " << this->rate << "\n";
        }
        else if(it == 1 && this->verbose)
            cout << "RATE: " << this->rate << "\n";

        secs = stime/1000;
        if(this->verbose) cout << " " << it+1 << "        " << this->steps << "           " << this->ctot << "              " << rmargin << "            " << norm << "           " << secs << " ";

        ++it; //IMA iteration increment

        imapFixMargin.setGamma(gamma);
        imapFixMargin.setSolution(tempSol);
        imapFixMargin.setLearningRate(this->rate);

        //  break;
        if(flagNao1aDim) break;
    }
    for(min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i)
    {
        y = points[i]->y;
        alpha = points[i]->alpha;
         if(alpha > this->EPS * this->rate) { this->svs.push_back(i); }        
    }
    this->steps = imapFixMargin.getSteps();
    this->ctot = imapFixMargin.getCtot();
    this->solution.w = w_saved;
    this->solution.margin = rmargin;
    this->solution.norm = norm;
    this->solution.bias = bias;
    this->solution.svs = this->svs.size();

    if(this->verbose)
    {
        cout << "\n-----------------------------------------------------------------------------\n";
        cout << "Number of times that the Fixed Margin Perceptron was called: " << it+1 << "\n";
        cout << "Number of steps through data: " << this->steps <<"\n";
        cout << "Number of updates: " << this->ctot << "\n";
        cout << "Margin found: " << rmargin << "\n";
        cout << "Min: " << fabs(min) << " / Max: " << fabs(max) << "\n";
        cout << "Number of Support Vectors: " << this->svs.size()  << "\n\n";
        if(this->verbose >= 2)
        {
            for(i = 0; i < dim; ++i) cout << "W[" << fnames[i] << "]: " << w_saved[i] << "\n";
            cout << "Bias: " << this->solution.bias << "\n\n";
        }
    }

    this->samples->resetIndex();

    if(!it)
    {
        if(this->verbose) cout << "FMP convergency wasn't reached!\n";
        return 0;
    }
    return 1;
}

template < typename T >
double IMAp< T >::evaluate(Point< T > p) {
    double func = 0.0;
    int i;
    size_t dim = this->solution.w.size();

    if(p.x.size() != dim){
        cerr << "The point must have the same dimension of the feature set!" << endl;
        return 0;
    }

    for(func = this->solution.bias, i = 0; i < dim; i++){
        func += this->solution.w[i] * p[i];
    }

    return (func >= this->solution.margin * this->solution.norm)?1:-1;
}

template < typename T >
IMApFixedMargin< T >::IMApFixedMargin(std::shared_ptr<Data< T > > samples, double gamma, Solution *initial_solution) {
    this->gamma = gamma;
    this->samples = samples;

    if(initial_solution){
        this->w = initial_solution->w;
        this->solution.bias = initial_solution->bias;
        this->solution.norm = initial_solution->norm;
    }else{
        this->w.resize(samples->getDim());
        this->solution.func.resize(samples->getSize());
    }
}

template < typename T >
bool IMApFixedMargin< T >::train() {
    int c, e = 1, i, k, s = 0, j;
    int t, idx, r;
    size_t size = this->samples->getSize(), dim = this->samples->getDim();
    double norm = this->solution.norm , bias = this->solution.bias, lambda = 1, y, time = this->max_time + this->start_time;
    register double sumnorm = 0; //soma das normas para o calculo posterior (nao mais sqrt)
    double maiorw_temp = 0;
    int n_temp, sign= 1;
    bool cond;
    vector<double> func(size, 0.0);
    vector<int> index = this->samples->getIndex();
    vector< T > x;

    while(this->timer.Elapsed() - time <= 0)
    {
        for(e = 0, i = 0; i < size; ++i)
        {
            //shuffling data r = i + rand()%(size-i); j = index[i]; idx = index[i] = index[r]; index[r] = j;
            idx = index[i];
            //cout << idx << endl;
            x = (*this->samples)[idx]->x;
            y = (*this->samples)[idx]->y;
            //if(i == 100) return 1;
            //calculating function
            for(func[idx] = bias, j = 0; j < dim; ++j){
                func[idx] += this->w[j] * x[j];
            }

            //cout << "funcidx: " << y*func[idx] << " marg: " << this->gamma*norm - points[idx]->alpha*this->flexible <<"\n ";
            //Checking if the point is a mistake
            if(y*func[idx] <= this->gamma*norm - (*this->samples)[idx]->alpha*this->flexible)
            {
                lambda = (norm) ? (1-this->rate*this->gamma/norm) : 1;
                for(r = 0; r < size; ++r)
                    (*this->samples)[r]->alpha *= lambda;

                if(this->q == 1.0) //Linf
                {
                    for(sumnorm = 0, j = 0; j < dim; ++j)
                    {
                        sign = 1; if(this->w[j] < 0) sign = -1;
                        lambda = (norm > 0 && this->w[j] != 0) ? this->gamma * sign: 0;
                        this->w[j] += this->rate * (y * x[j] - lambda);
                        sumnorm += fabs(this->w[j]);
                    }
                    norm = sumnorm;
                }
                else if(this->q == 2.0) //L2
                {
                    for(sumnorm = 0, j = 0; j < dim; ++j)
                    {
                        lambda = (norm > 0 && this->w[j] != 0) ? this->w[j] * this->gamma / norm : 0;
                        this->w[j] += this->rate * (y * x[j] - lambda);
                        sumnorm += this->w[j] * this->w[j];
                    }
                    norm = sqrt(sumnorm);
                }
                else if(this->q == -1.0) //L1
                {
                    maiorw_temp = fabs(this->w[0]);
                    n_temp = 1;
                    for(j = 0; j < dim; ++j)
                    {
                        if(this->maiorw == 0 || fabs(this->maiorw - fabs(this->w[j]))/this->maiorw < this->EPS)
                        {
                            sign = 1; if(this->w[j] < 0) sign = -1;
                            lambda = (norm > 0 && this->w[j] != 0) ? this->gamma * sign / this->n : 0;
                            this->w[j] += this->rate * (y * x[j] - lambda);
                        }
                        else
                            this->w[j] += this->rate * (y * x[j]);

                        if(j > 0)
                        {
                            if(fabs(maiorw_temp - fabs(this->w[j]))/maiorw_temp < this->EPS)
                                n_temp++;
                            else if(fabs(this->w[j]) > maiorw_temp)
                            {
                                maiorw_temp = fabs(this->w[j]);
                                n_temp = 1;
                            }
                        }
                    }
                    this->maiorw = maiorw_temp;
                    this->n = n_temp;
                    norm = this->maiorw;
                    if(this->n > this->maiorn) this->maiorn = this->n;
                }
                else //outras formula��es - Lp
                {
                    for(sumnorm = 0, j = 0; j < dim; ++j)
                    {
                        lambda = (norm > 0 && this->w[j] != 0) ? this->w[j] * this->gamma * pow(fabs(this->w[j]), this->q-2.0) * pow(norm, 1.0-this->q) : 0;
                        this->w[j] += this->rate * (y * x[j] - lambda);
                        sumnorm += pow(fabs(this->w[j]), this->q);
                    }
                    norm = pow(sumnorm, 1.0/this->q);
                }
                bias += this->rate * y;
                (*this->samples)[idx]->alpha += this->rate;

                k = (i > s) ? s++ : e;
                j = index[k];
                index[k] = idx;
                index[i] = j;
                this->ctot++; e++;
            }
            else if(this->steps > 0 && e > 1 && i > s) break;
        }
        this->steps++; //Number of iterations update
        //cout << e << endl;
        //stop criterion
        if(e == 0)     break;
        if(this->steps > this->MAX_IT) break;
        if(this->ctot > this->MAX_UP) break;
        if(this->flagNao1aDim) if(this->ctot > tMax) break;
    }

    this->samples->setIndex(index);
    this->solution.norm = norm;
    this->solution.bias = bias;
    this->solution.w = this->w;
    this->solution.func = func;

    if(e == 0) return 1;
    else       return 0;
}

template < typename T >
double IMApFixedMargin< T >::evaluate(Point< T > p) {
    double func = 0.0;
    int i;
    size_t dim = this->solution.w.size();

    if(p.x.size() != dim){
        cerr << "The point must have the same dimension of the feature set!" << endl;
        return 0;
    }

    for(func = this->solution.bias, i = 0; i < dim; i++){
        func += this->solution.w[i] * p[i];
    }

    return (func >= this->solution.margin * this->solution.norm)?1:-1;
}

template < typename T >
IMADual< T >::IMADual(std::shared_ptr<Data< T > > samples, Kernel *k, double rate, Solution *initial_solution) {
    this->samples = samples;
    this->kernel = k;
    this->rate = rate;

    if(initial_solution){
        this->solution.w = initial_solution->w;
        this->solution.bias = initial_solution->bias;
        this->hasInitialSolution = true;
    }else{
        this->solution.w.resize(samples->getDim());
    }
}

template < typename T >
bool IMADual< T >::train() {
    register int it;
    double rmargin = 0, secs;

    register int i, j;
    size_t sv = 0, size = this->samples->getSize(), dim = this->samples->getDim();
    double min, max, norm = 0, stime = 0;
    dMatrix K;
    vector<int> index = this->samples->getIndex();
    vector<double> w_saved(dim), saved_alphas(size), func(size);
    vector<shared_ptr<Point< T > > > points = this->samples->getPoints();

    this->timer.Reset();

    //Allocating space for index
    if(index.size() == 0)
    {
        index.resize(size);

        //Initializing alpha and bias
        for(i = 0; i < size; ++i) { index[i] = i; }
    }
    this->solution.bias = 0;

    //Allocating space kernel matrix
    this->kernel->compute(this->samples);

    if(this->verbose)
    {
        cout << "-------------------------------------------------------------------\n";
        cout << "  steps      updates        margin          norm       svs     secs\n";
        cout << "-------------------------------------------------------------------\n";
    }

    it = 0; this->ctot = 0; this->steps = 0; this->gamma = 0;

    PerceptronFixedMarginDual< T > percDual(this->samples, this->gamma, this->rate, this->kernel, nullptr);
    Solution sol, *solr;

    percDual.setLearningRate(this->rate);
    percDual.setMaxTime(this->max_time);

    stime = this->timer.Elapsed();

    while(percDual.train()){
        stime += percDual.getElapsedTime();

        //Finding minimum and maximum functional values
        this->ctot = percDual.getCtot();
        this->steps = percDual.getSteps();
        solr = percDual.getSolutionRef();
        norm = solr->norm;
        this->solution.bias = solr->bias;

        for(sv = 0, min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i)
        {
            if(points[i]->alpha > this->EPS*this->rate) { sv++; saved_alphas[i] = points[i]->alpha; }
            else                           { saved_alphas[i] = 0.0; }
            if(solr->func[i] >= 0 && min > solr->func[i]/norm) min = solr->func[i]/norm;
            else if(solr->func[i] <  0 && max < solr->func[i]/norm) max = solr->func[i]/norm;
        }

        //Obtaining real margin
        rmargin = (fabs(min) > fabs(max)) ? fabs(max) : fabs(min);

        //Obtaining new gamma_f
        this->gamma = (min-max)/2.0;
        if(this->gamma < this->MIN_INC*rmargin) this->gamma = this->MIN_INC*rmargin;

        percDual.setGamma(this->gamma);
        secs = stime / 1000;

        if(this->verbose) cout << "    " << this->steps <<"         " << this->ctot << "          " << rmargin << "         " << norm << "     " << sv <<"     "<< secs << endl;
        ++it; //IMA iteration increment
    }

    this->ctot = percDual.getCtot();
    this->steps = percDual.getSteps();
    sol = percDual.getSolution();
    this->alpha = percDual.getAlphaVector();
    norm = sol.norm;
    this->solution.bias = sol.bias;
    func = sol.func;

    for(i = 0; i < size; ++i)
    {
        if(points[i]->alpha > this->EPS*this->rate) { this->svs.push_back(i);}
    }

    for(i = 0; i < size; ++i) points[i]->alpha = saved_alphas[i];

    this->solution.norm = this->kernel->norm(*this->samples);

    /*recuperando o vetor DJ -- "pesos" das componentes*/
    int kernel_type = this->kernel->getType();
    double kernel_param = this->kernel->getParam();

    if(kernel_type == 0)
        for(i = 0; i < dim; i++){
            for(j = 0; j < size; j++){
                this->solution.w[i] += points[j]->alpha*points[j]->y*points[j]->x[i];
            }
        }
    else
    {
        if(kernel_type == 1 && kernel_param == 1)
            w_saved = DualClassifier< T >::getDualWeightProdInt();
        else
            w_saved = DualClassifier< T >::getDualWeight();
        if(it) Data< T >::normalize(this->solution.w, 2);
    }

    this->solution.margin = rmargin;

    if(this->verbose)
    {
        cout << "-------------------------------------------------------------------\n";
        cout << "Number of times that the Fixed Margin Perceptron was called:: " << it+1 << endl;
        cout << "Number of steps through data: " << this->steps << endl;
        cout << "Number of updates: " << this->ctot << endl;
        cout << "Number of support vectors: " << sv << endl;
        cout << "Margin found: " << rmargin << "\n\n";
        if(this->verbose > 2)
        {
            vector<int> fnames = this->samples->getFeaturesNames();
            for(i = 0; i < dim; i++)
                cout << "W[" << fnames[i] << "]: " << this->solution.w[i] << endl;
            cout << "Bias: " << this->solution.bias << "\n\n";
        }
    }

    if(!it)
    {
        if(this->verbose) cout << "FMP convergency wasn't reached!\n";
        return 0;
    }
    return true;
}

template < typename T >
double IMADual< T >::evaluate(Point< T > p) {
    double func, bias = this->solution.bias, fk = 0.0, lambda;
    size_t size = this->samples->getSize(), dim = this->samples->getDim(), r;
    auto po = make_shared<Point< T > >(p);

    if(p.x.size() != dim){
        cerr << "The point must have the same dimension of the feature set!" << endl;
        return 0;
    }

    for(func = bias, r = 0; r < size; ++r){
        fk = this->kernel->function(po, (*this->samples)[r], dim);
        func  += (*this->samples)[r]->alpha * (*this->samples)[r]->y * fk;
    }
    return (func >= 0)?1:-1;
}

template class IMAp<int>;
template class IMAp<double>;
template class IMAp<float>;
template class IMAp<int8_t>;
template class IMAp<char>;
template class IMAp<long long int>;
template class IMAp<short int>;
template class IMAp<long double>;
template class IMAp<unsigned char>;
template class IMAp<unsigned int>;
template class IMAp<unsigned short int>;

template class IMApFixedMargin<int>;
template class IMApFixedMargin<double>;
template class IMApFixedMargin<float>;
template class IMApFixedMargin<int8_t>;
template class IMApFixedMargin<char>;
template class IMApFixedMargin<long long int>;
template class IMApFixedMargin<short int>;
template class IMApFixedMargin<long double>;
template class IMApFixedMargin<unsigned char>;
template class IMApFixedMargin<unsigned int>;
template class IMApFixedMargin<unsigned short int>;

template class IMADual<int>;
template class IMADual<double>;
template class IMADual<float>;
template class IMADual<int8_t>;
template class IMADual<char>;
template class IMADual<long long int>;
template class IMADual<short int>;
template class IMADual<long double>;
template class IMADual<unsigned char>;
template class IMADual<unsigned int>;
template class IMADual<unsigned short int>;