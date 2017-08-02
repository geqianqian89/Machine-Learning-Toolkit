//
// Created by mateus558 on 01/08/17.
//

#include <iostream>
#include <cmath>
#include "../includes/IMA.hpp"

using namespace std;

IMAp::IMAp(Data *samples, double margin, Solution *initial_solution = NULL) {
    this->samples = samples;
    this->margin = margin;

    if(initial_solution){
        w = initial_solution->w;
        solution.bias = initial_solution->bias;
    }else{
        w.resize(samples->getSize());
    }
}

bool IMAp::train() {
    long int tmax = 0;
    int i, j, n, maiorn = 0, flagNao1aDim = 0, y, it;
    int size = samples->getSize(), dim = samples->getDim(), t1=1, t3=1;
    double gamma, secs, bias = 0.0, alpha, rmargin = margin, inc;
    double min = 0.0, max = 0.0, norm = 1.0, maiorw = 0.0;
    vector<double> w_saved, func;
    vector<int> index = samples->getIndex(), fnames = samples->getFeaturesNames();
    vector<Point> points = samples->getPoints();

    n = dim;
    rate = 1.0;
    //Initializing data struct
    solution.norm  = 1.0;

    //Allocating space for w_saved and func
    w_saved.resize(dim);
    func.resize(size);

    //Allocating space for w
    if(w.size() > 0) {
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
            y = points[i].y;
            for (func[i] = 0, j = 0; j < dim; ++j)
                func[i] += w[j] * points[i].x[j];
            if (y == 1 && func[i] < min) min = func[i];
            else if (y == -1 && func[i] > max) max = func[i];
        }
        //printf("min = %lf\n", min);
        //printf("max = %lf\n", max);
        //printf("flag = %d\n", flag);
        solution.bias = -(min + max) / 2.0;
        //printf("bias: %lf\n", data.z->bias);
        //flag = 0;
        for (min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i) {
            y = points[i].y;
            for (func[i] = solution.bias, j = 0; j < dim; ++j)
                func[i] += w[j] * points[i].x[j];
            if (func[i] * y < 0) flag++;
            if (y == 1 && func[i] < min) min = func[i];
            else if (y == -1 && func[i] > max) max = func[i];
        }
        //printf("flag = %d\n", flag);
        if (flag) rmargin = 0;
        else rmargin = fabs(min);
        //printf("*margin = %lf / rmargin = %lf\n", *margin, rmargin);
        if (margin == 0) tMax = MAX_UP;
        else {
            double raio = Statistics::getRadius(*samples, -1, q);//data_get_radius(sample, -1, q);
            tMax = (raio * raio - rmargin * rmargin) / pow(margin - rmargin, 2);
            if (rmargin == 0) tMax *= 1.5;
            //tMax *= 2;
        }
        //printf("tMax = %ld\n", tMax);
        //rmargin = 0;
        //tMax = (raio * raio - rmargin * rmargin) / pow(*margin - rmargin, 2);
        //printf("tMax = %d\n", tMax);
    }

    //Allocating space for index and initializing
    if(index.size() == 0)
    {
        index.resize(size);
        for(i = 0; i < size; ++i) index[i] = i;
    }
    samples->setIndex(index);

    //Initializing w_saved and func
    for(i = 0; i <  dim; ++i) w_saved[i] = 0.0;
    for(i = 0; i < size; ++i) { func[i] = 0.0; points[i].alpha = 0.0; }

    if(verbose)
    {
        cout << "----------------------------------------------------------------------\n";
        cout << " pmf    passos     atualiz.        margem            norma        segs\n";
        cout << "----------------------------------------------------------------------\n";
    }

    it = 0; ctot = 0; steps = 0; gamma = 0.0; //gammaf1 = 0.0; t1 = 0; gammaf2 = 0.0; t2 = 0; gammaf3 = 0.0; t3 = 0; Cmedia = 0.0;
    //srand(0); //zerar a semente pros resultados serem sempre iguais
    start_time = 100.0f*clock()/CLOCKS_PER_SEC;

    //Parei aqui
    while(imap_fixed_margin_perceptron(&data, gamma, &steps, &ctot, sample->index, q, max_time))
    {
        //Finding minimum and maximum functional values
        norm  = solution.norm;
        bias  = solution.bias;
        for(min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i)
        {
            y = points[i].y;
            alpha = points[i].alpha;
            if((func[i] + y*alpha*flexible) >= 0 && min > (func[i] + y*alpha*flexible)/norm) min = (func[i] + y*alpha*flexible)/norm;
            else if((func[i] + y*alpha*flexible) <  0 && max < (func[i] + y*alpha*flexible)/norm) max = (func[i] + y*alpha*flexible)/norm;
        }

        //Saving good weights
        for(i = 0; i < dim; i++) w_saved[i] = solution.w[i];

        //Obtaining real margin
        rmargin = (fabs(min) > fabs(max)) ? fabs(max) : fabs(min);

        //Shift no bias
        double mmargin = (fabs(max) + fabs(min)) / 2.0;
        if(fabs(max) > fabs(min))
            solution.bias += fabs(mmargin - rmargin);
        else
            solution.bias -= fabs(mmargin - rmargin);

        //Obtaining new gamma_f
        gamma = (min-max)/2.0;
        inc = (1+sample->alpha_aprox)*rmargin;
        if(gamma < inc) gamma = inc;
        rmargin = mmargin;

        if(it == 2)
            t1 = ctot;
        t3 = ctot;

        if(it > 1)
        {
            RATE = sqrt(t1) / sqrt(t3);
            if(verbose) cout << "RATE: " << rate << "\n";
        }
        else if(it == 1 && verbose)
            cout << "RATE: " << rate << "\n";

        secs = (100.0f*clock()/CLOCKS_PER_SEC-start_time)/100.0f;
        if(verbose) cout << " " << it+1 << "   " << steps << "    " << ctot << "    " << rmargin << "    " << norm << "    " << secs << " ";

        ++it; //IMA iteration increment
        //if(it > 3)
        if(flagNao1aDim) break;
    }

    solution.w = w_saved;
    solution.margin = rmargin;
    solution.norm = norm;
    solution.bias = bias;

    if(verbose)
    {
        cout << "\n----------------------------------------------------------------------\n";
        cout << "Numero de vezes que o Perceptron de Margem Fixa foi chamado: " << it+1 << "\n";
        cout << "Numero de passos atraves dos dados: " << steps <<"\n";
        cout << "Numero de atualizacoes: " << ctot << "\n";
        cout << "Margem encontrada: " << rmargin << "\n";
        cout << "Min: " << fabs(min) << " / Max: " << fabs(max) << "\n\n";
        if(verbose > 1)
        {
            for(i = 0; i < dim; ++i) cout << "W[" << fnames[i] << "]: " << w_saved[i] << "\n";
            cout << "Bias: " << solution.bias << "\n\n";
        }
    }

    if(!it)
    {
        if(verbose) cout << "Convergencia do FMP nao foi atingida!\n";
        return 0;
    }
    return 1;
}