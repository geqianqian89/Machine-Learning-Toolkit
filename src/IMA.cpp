//
// Created by mateus558 on 01/08/17.
//

#include <iostream>
#include <cmath>
#include <float.h>
#include "../includes/IMA.hpp"
#include "../includes/Timer.hpp"
#include "../includes/Perceptron.hpp"

using namespace std;

IMAp::IMAp(Data *samples, double margin,  Solution *initial_solution) {
    this->samples = samples;
    this->margin = margin;

    if(initial_solution){
        w = initial_solution->w;
        solution.bias = initial_solution->bias;
        hasInitialSolution = true;
    }else{
        w.resize(samples->getDim());
    }
}

bool IMAp::train() {
    unsigned int tMax = 0;
    int i, j, n, maiorn = 0, flagNao1aDim = 0, y, it, sign = 1;
    int size = samples->getSize(), dim = samples->getDim(), t1=1, t3=1;
    double gamma, secs, bias = 0.0, alpha, rmargin = margin, inc;
    double min = 0.0, max = 0.0, norm = 1.0, maiorw = 0.0;
    vector<double> w_saved, func;
    vector<int> index = samples->getIndex(), fnames = samples->getFeaturesNames();
    vector<shared_ptr<Point> > points = samples->getPoints();
    IMApFixedMargin imapFixMargin(samples, gamma);
    Solution tempSol;

    n = dim;
    rate = 1.0;
    //Initializing data struct
    solution.norm  = 1.0;

    //Allocating space for w_saved and func
    w_saved.resize(dim);
    func.resize(size);

    //Allocating space for w
    if(hasInitialSolution) {
        if (q == 1)
            for (solution.norm = 0.0, i = 0; i < dim; ++i) solution.norm += fabs(w[i]);
        else if (q == 2) {
            for (solution.norm = 0.0, i = 0; i < dim; ++i) solution.norm += w[i] * w[i];
            solution.norm = sqrt(solution.norm);
        } else {
            for (solution.norm = 0.0, i = 0; i < dim; ++i) solution.norm += pow(fabs(w[i]), q);
            solution.norm = pow(solution.norm, 1.0 / q);
        }

        for (i = 0; i < dim; ++i) w[i] /= solution.norm;

        solution.bias /= solution.norm;
        solution.norm = 1;
        flagNao1aDim = 1;
        int flag = 0;

        for (min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i) {
            y = points[i]->y;
            for (func[i] = 0, j = 0; j < dim; ++j)
                func[i] += w[j] * points[i]->x[j];
            if (y == 1 && func[i] < min) min = func[i];
            else if (y == -1 && func[i] > max) max = func[i];
        }

        solution.bias = -(min + max) / 2.0;

        for (min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i) {
            y = points[i]->y;
            for (func[i] = solution.bias, j = 0; j < dim; ++j)
                func[i] += w[j] * points[i]->x[j];
            if (func[i] * y < 0) flag++;
            if (y == 1 && func[i] < min) min = func[i];
            else if (y == -1 && func[i] > max) max = func[i];
        }

        if (flag) rmargin = 0;
        else rmargin = fabs(min);

        if (margin == 0) tMax = MAX_UP;
        else {
            double raio = Statistics::getRadius(*samples, -1, q);//data_get_radius(sample, -1, q);
            tMax = (raio * raio - rmargin * rmargin) / pow(margin - rmargin, 2);
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
    samples->setIndex(index);

    //Initializing alpha
    for(i = 0; i < size; ++i) { points[i]->alpha = 0.0; }

    if(verbose)
    {
        cout << "-----------------------------------------------------------------------------\n";
        cout << " pmf    steps     updates              margin              norm          secs\n";
        cout << "-----------------------------------------------------------------------------\n";
    }

    it = 0; ctot = 0; steps = 0; gamma = 0.0;

    imapFixMargin.setCtot(ctot);
    imapFixMargin.setqNorm(q);
    imapFixMargin.setSteps(steps);
    imapFixMargin.setGamma(gamma);
    imapFixMargin.setFlexible(flexible);
    imapFixMargin.setLearningRate(rate);

    *imapFixMargin.getFlagNot1aDim() = flagNao1aDim;

    while(imapFixMargin.train())
    {
        ctot = imapFixMargin.getCtot();
        steps = imapFixMargin.getSteps();
        //Finding minimum and maximum functional values
        tempSol = imapFixMargin.getSolution();
        norm  = tempSol.norm;
        bias  = tempSol.bias;
        func = tempSol.func;

        for(min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i)
        {
            y = points[i]->y;
            alpha = points[i]->alpha;
            if((func[i] + y*alpha*flexible) >= 0 && min > (func[i] + y*alpha*flexible)/norm) min = (func[i] + y*alpha*flexible)/norm;
            else if((func[i] + y*alpha*flexible) <  0 && max < (func[i] + y*alpha*flexible)/norm) max = (func[i] + y*alpha*flexible)/norm;
        }
        //Saving good weights
        for(i = 0; i < dim; i++) w_saved[i] = tempSol.w[i];
        //Compute real margin
        rmargin = (fabs(min) > fabs(max)) ? fabs(max) : fabs(min);

        //Shift no bias
        double mmargin = (fabs(max) + fabs(min)) / 2.0;
        if(fabs(max) > fabs(min))
            tempSol.bias += fabs(mmargin - rmargin);
        else
            tempSol.bias -= fabs(mmargin - rmargin);

        //Obtaining new gamma_f
        gamma = (min-max)/2.0;
        inc = (1+alpha_aprox)*rmargin;
        if(gamma < inc) gamma = inc;
        rmargin = mmargin;

        if(it == 2)
            t1 = ctot;
        t3 = ctot;

        if(it > 1)
        {
            rate = sqrt(t1) / sqrt(t3);
            if(verbose) cout << "\tRATE: " << rate << "\n";
        }
        else if(it == 1 && verbose)
            if(verbose) cout << "\tRATE: " << rate << "\n";

        secs = imapFixMargin.getElapsedTime()/1000;
        if(verbose) cout << " " << it+1 << "        " << steps << "           " << ctot << "              " << rmargin << "            " << norm << "           " << secs << " ";

        ++it; //IMA iteration increment

        imapFixMargin.setCtot(ctot);
        imapFixMargin.setSteps(steps);
        imapFixMargin.setGamma(gamma);
        imapFixMargin.setSolution(tempSol);
        imapFixMargin.setLearningRate(rate);

        //  break;
        if(flagNao1aDim) break;
    }

    solution.w = w_saved;
    solution.margin = rmargin;
    solution.norm = norm;
    solution.bias = bias;

    if(verbose)
    {
        cout << "\n-----------------------------------------------------------------------------\n";
        cout << "Number of times that the Fixed Margin Perceptron was called: " << it+1 << "\n";
        cout << "Number of steps through data: " << steps <<"\n";
        cout << "Number of updates: " << ctot << "\n";
        cout << "Margin found: " << rmargin << "\n";
        cout << "Min: " << fabs(min) << " / Max: " << fabs(max) << "\n\n";
        if(verbose == 3)
        {
            for(i = 0; i < dim; ++i) cout << "W[" << fnames[i] << "]: " << w_saved[i] << "\n";
            cout << "Bias: " << solution.bias << "\n\n";
        }
    }

    samples->resetIndex();

    if(!it)
    {
        if(verbose) cout << "FMP convergency wasn't reached!\n";
        return 0;
    }
    return 1;
}

double IMAp::evaluate(Point p) {
    return 0;
}

IMApFixedMargin::IMApFixedMargin(Data *samples, double gamma, Solution *initial_solution) {
    this->gamma = gamma;
    this->samples = samples;

    if(initial_solution){
        w = initial_solution->w;
        solution.bias = initial_solution->bias;
        solution.norm = initial_solution->norm;
    }else{
        w.resize(samples->getDim());
        solution.func.resize(samples->getSize());
    }
}

bool IMApFixedMargin::train() {
    int c, e, i, k, s, j;
    int t, idx, r;
    int size = samples->getSize(), dim = samples->getDim();
    double norm = solution.norm , bias = solution.bias, lambda = 1, y, time = max_time;
    register double sumnorm = 0; //soma das normas para o calculo posterior (nao mais sqrt)
    double maiorw_temp = 0;
    int n_temp, sign= 1;
    bool cond;
    vector<double> x, func(size, 0.0);
    vector<int> index = samples->getIndex();
    vector<shared_ptr<Point> > points = samples->getPoints();
    e = 1,s = 0;

    timer.Reset();
    start_time = timer.Elapsed();
    time += start_time;
    while(timer.Elapsed() - time <= 0)
    {
        for(e = 0, i = 0; i < size; ++i)
        {
            //shuffling data r = i + rand()%(size-i); j = index[i]; idx = index[i] = index[r]; index[r] = j;
            idx = index[i];
            x = points[idx]->x;
            y = points[idx]->y;

            //calculating function
            for(func[idx] = bias, j = 0; j < dim; ++j)
                func[idx] += w[j] * x[j];

            //Checking if the point is a mistake
            if(y*func[idx] <= gamma*norm - points[idx]->alpha*flexible)
            {
                lambda = (norm) ? (1-rate*gamma/norm) : 1;
                for(r = 0; r < size; ++r)
                    points[r]->alpha *= lambda;

                if(q == 1.0) //Linf
                {
                    for(sumnorm = 0, j = 0; j < dim; ++j)
                    {
                        sign = 1; if(w[j] < 0) sign = -1;
                        lambda = (norm > 0 && w[j] != 0) ? gamma * sign: 0;
                        w[j] += rate * (y * x[j] - lambda);
                        sumnorm += fabs(w[j]);
                    }
                    norm = sumnorm;
                }
                else if(q == 2.0) //L2
                {
                    for(sumnorm = 0, j = 0; j < dim; ++j)
                    {
                        lambda = (norm > 0 && w[j] != 0) ? w[j] * gamma / norm : 0;
                        w[j] += rate * (y * x[j] - lambda);
                        sumnorm += w[j] * w[j];
                    }
                    norm = sqrt(sumnorm);
                }
                else if(q == -1.0) //L1
                {
                    maiorw_temp = fabs(w[0]);
                    n_temp = 1;
                    for(j = 0; j < dim; ++j)
                    {
                        if(maiorw == 0 || fabs(maiorw - fabs(w[j]))/maiorw < EPS)
                        {
                            sign = 1; if(w[j] < 0) sign = -1;
                            lambda = (norm > 0 && w[j] != 0) ? gamma * sign / n : 0;
                            w[j] += rate * (y * x[j] - lambda);
                        }
                        else
                            w[j] += rate * (y * x[j]);

                        if(j > 0)
                        {
                            if(fabs(maiorw_temp - fabs(w[j]))/maiorw_temp < EPS)
                                n_temp++;
                            else if(fabs(w[j]) > maiorw_temp)
                            {
                                maiorw_temp = fabs(w[j]);
                                n_temp = 1;
                            }
                        }
                    }
                    maiorw = maiorw_temp;
                    n = n_temp;
                    norm = maiorw;
                    if(n > maiorn) maiorn = n;
                }
                else //outras formula\E7\F5es - Lp
                {
                    for(sumnorm = 0, j = 0; j < dim; ++j)
                    {
                        lambda = (norm > 0 && w[j] != 0) ? w[j] * gamma * pow(fabs(w[j]), q-2.0) * pow(norm, 1.0-q) : 0;
                        w[j] += rate * (y * x[j] - lambda);
                        sumnorm += pow(fabs(w[j]), q);
                    }
                    norm = pow(sumnorm, 1.0/q);
                }
                bias += rate * y;
                points[idx]->alpha += rate;

                k = (i > s) ? s++ : e;
                j = index[k];
                index[k] = idx;
                index[i] = j;
                ctot++; e++;

            }else if(t > 0 && e > 1 && i > s) break;
        }
        steps++; //Number of iterations update
        //stop criterion
        if(e == 0)     break;
        if(steps > MAX_IT) break;
        if(ctot > MAX_UP) break;
        if(flagNao1aDim) if(ctot > tMax) break;
    }

    samples->setIndex(index);
    solution.norm = norm;
    solution.bias = bias;
    solution.w = w;
    solution.func = func;

    if(e == 0) return 1;
    else       return 0;
}

double IMApFixedMargin::evaluate(Point p) {
    return 0;
}

IMADual::IMADual(Data *samples, Kernel *k, double rate, Solution *initial_solution) {
    this->samples = samples;
    this->kernel = k;
    this->rate = rate;

    if(initial_solution){
        solution.w = initial_solution->w;
        solution.bias = initial_solution->bias;
        hasInitialSolution = true;
    }else{
        solution.w.resize(samples->getDim());
    }
}

bool IMADual::train() {
    register int it;
    double rmargin = 0, secs;

    register int i, j;
    int sv = 0, size = samples->getSize(), dim = samples->getDim();
    double min, max, norm = 0;
    dMatrix K;
    vector<int> index = samples->getIndex();
    vector<double> w_saved(dim), saved_alphas(size), func(size);
    vector<shared_ptr<Point> > points = samples->getPoints();

    //Allocating space for index
    if(index.size() == 0)
    {
        index.resize(size);

        //Initializing alpha and bias
        for(i = 0; i < size; ++i) { index[i] = i; }
    }
    solution.bias = 0;

    //Allocating space kernel matrix
    kernel->compute(*samples);

    if(verbose)
    {
        cout << "-------------------------------------------------------------------\n";
        cout << "  steps      updates        margin          norm       svs     secs\n";
        cout << "-------------------------------------------------------------------\n";
    }

    it = 0; ctot = 0; steps = 0; gamma = 0;
    double start_time = 100.0f*clock()/CLOCKS_PER_SEC;

    PerceptronFixedMarginDual percDual(samples, gamma, rate, kernel, nullptr);
    Solution sol;

    while(true){
        if(!percDual.train()) break;

        //Finding minimum and maximum functional values
        ctot = percDual.getCtot();
        steps = percDual.getSteps();
        sol = percDual.getSolution();
        norm = sol.norm;
        solution.bias = sol.bias;
        func = sol.func;

        for(sv = 0, min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i)
        {
            if(points[i]->alpha > EPS*rate) { sv++; saved_alphas[i] = points[i]->alpha; }
            else                           { saved_alphas[i] = 0.0; }
            if(func[i] >= 0 && min > func[i]/norm) min = func[i]/norm;
            else if(func[i] <  0 && max < func[i]/norm) max = func[i]/norm;
        }

        //Obtaining real margin
        rmargin = (fabs(min) > fabs(max)) ? fabs(max) : fabs(min);

        //Obtaining new gamma_f
        gamma = (min-max)/2.0;
        if(gamma < MIN_INC*rmargin) gamma = MIN_INC*rmargin;

        percDual.setCtot(ctot);
        percDual.setSteps(steps);
        percDual.setGamma(gamma);
        percDual.setSolution(sol);

        secs = (100.0f*clock()/CLOCKS_PER_SEC-start_time)/100.0f;

        if(verbose) cout << "    " << steps <<"         " << ctot << "          " << rmargin << "         " << norm << "     " << sv <<"     "<< secs << endl;
        ++it; //IMA iteration increment
    }

    for(i = 0; i < size; ++i) points[i]->alpha = saved_alphas[i];

    solution.norm = kernel->norm(*samples);

    /*recuperando o vetor DJ -- "pesos" das componentes*/
    int kernel_type = kernel->getType();
    double kernel_param = kernel->getParam();

    if(kernel_type == 0)
        for(i = 0; i < dim; i++){
            for(j = 0; j < size; j++){
                solution.w[i] += alpha[j]*points[j]->y*points[j]->x[i];
            }
        }
    else
    {
        if(kernel_type == 1 && kernel_param == 1)
            cout << endl;
           // w_saved = utils_get_dualweight_prodint(sample);
        else
            cout <<"i" <<endl;
            //w_saved = utils_get_dualweight(sample);
        if(it) Data::normalize(solution.w, 2);
    }

    solution.margin = rmargin;

    if(verbose)
    {
        cout << "-------------------------------------------------------------------\n";
        cout << "Number of times that the Fixed Margin Perceptron was called:: " << it+1 << endl;
        cout << "Number of steps through data: " << steps << endl;
        cout << "Number of updates: " << ctot << endl;
        cout << "Number of support vectors: " << sv << endl;
        cout << "Margin found: " << rmargin << "\n\n";
        /*if(verbose > 1)
        {
            for(i = 0; i < dim; i++)
                printf("W[%d]: %lf\n", sample->fnames[i], w_saved[i]);
            printf("Bias: %lf\n\n", sample->bias);
        }*/
    }

    if(!it)
    {
        if(verbose) cout << "FMP convergency wasn't reached!\n";
        return 0;
    }
    return true;
}

double IMADual::evaluate(Point p) {
    return 0;
}
