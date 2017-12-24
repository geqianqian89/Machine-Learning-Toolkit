#include <vector>
#include <cmath>
#include <iostream>

#include "../includes/Statistics.hpp"
#include "../includes/Data.hpp"

using namespace std;

double Statistics::mean(vector<double> p){
    int i, psize = p.size();
    double avg;

    for(i = 0, avg = 0; i < psize; i++){
        avg += p[i];
    }

    return (avg / psize);
}

double Statistics::getFeatureMean(Data data, int index){
    int i, size = data.getSize();
    double sum = 0.0;
    vector<shared_ptr<Point> > points = data.getPoints();

    for(i = 0; i < size; ++i){
        sum += points[i]->x[index];
    }
    sum /= size;

    return sum;
}

double Statistics::variance(vector<double> p){
    if(p.size() == 1) return 0.0;
    int i;
    double avg, sum, dim = p.size();

    avg = mean(p);

    for(sum = 0.0, i = 0; i < dim; ++i){
        sum += (p[i] - avg)*(p[i] - avg);
    }

    return (sum / dim);
}

double Statistics::variance(Data data, int index){
    register int i, j;
    register double norm = 0.0;
    register double sum = 0.0;
    int dim = data.getDim(), size = data.getSize();
    vector<int> fnames = data.getFeaturesNames();
    vector<double> avg(dim);
    vector<shared_ptr<Point> > points = data.getPoints();

    for(j = 0; j < dim; ++j){
        if(index < 0 || fnames[j] != index){
            avg[j] = 0.0;

            for(i = 0; i < size; ++i){
                avg[j] += points[i]->x[j];
            }
            avg[j] = avg[j] / size;
        }
    }

    for(i = 0; i < size; ++i){
        for(j = 0; j < dim; ++j){
            if(index < 0 || fnames[j] != index){
                norm += pow(avg[j] - points[i]->x[j], 2);
            }
            sum += norm;
        }
    }

    sum = sum/size;

    return sum;
}

double Statistics::stdev(vector<double> p){
    return sqrt(variance(p));
}

double Statistics::getFeatureStdev(Data data, int index){
    int i, size = data.getSize();
    double avg, sd, vetsize = data.getDim();
    vector<shared_ptr<Point> > points = data.getPoints();

    if(size == 1) return 0.0;

    avg = getFeatureMean(data, index);

    for(sd = 0.0, i = 0; i < vetsize; ++i){
        sd = (points[i]->x[index] - avg)*(points[i]->x[index] - avg);
    }

    return sqrt(sd/(vetsize - 1));
}

double Statistics::getRadius(Data data, int index, double q){
    int i = 0, j = 0, dim = data.getDim(), size = data.getSize();
    double norm = 0.0;
    double max = 1.0;
    vector<int> fnames = data.getFeaturesNames();
    vector<double> avg(dim, 0.0);
    vector<shared_ptr<Point> > points = data.getPoints();

    if(q == 2){
        for(j = 0; j < dim; ++j){
            if(index < 0 || fnames[j] != index){
                for(i = 0; i < size; ++i){
                    avg[j] += points[i]->x[j];
                }
                avg[j] = avg[j] / size;
            }
        }

        for(max = 0, i = 0; i < size; ++i){
            for(norm = 0, j = 0; j < dim; ++j){
                if(index < 0 || fnames[j] != index){
                    norm += pow(avg[j] - points[i]->x[j], 2);
                }

                norm = sqrt(norm);

                if(max < norm) max = norm;
            }
        }

    }else if(q == 1){
        for(max = 0, i = 0; i < size; ++i){
            for(j = 0; j < dim; ++j){
                if(index < 0 || fnames[j] != index)
                    if(max < fabs(points[i]->x[j]))
                        max = fabs(points[i]->x[j]);
            }
        }
    }

    return max;
}

double Statistics::getDistCenters(Data data, int index){
    int i = 0, j = 0, dim = data.getDim(), size = data.getSize();
    double dist = 0.0;
    int size_pos = 0, size_neg = 0;
    vector<int> fnames = data.getFeaturesNames();
    vector<double> avg_pos(dim, 0.0), avg_neg(dim, 0.0);
    vector<shared_ptr<Point> > points = data.getPoints();

    for(size_pos = 0, size_neg = 0, i = 0; i < size; ++i){
        if(points[i]->y == 1)	size_pos++;
        else 					size_neg++;
    }

    for(j = 0; j < dim; ++j){
        for(i = 0; i < size; ++i){
            if(points[i]->y == 1){
                avg_pos[j] += points[i]->x[j];
            }else
                avg_neg[j] += points[i]->x[j];
        }

        avg_pos[j] /= (double)size_pos;
        avg_neg[j] /= (double)size_neg;
    }

    for(dist = 0.0, j = 0; j < dim; ++j){
        if(index < 0 || fnames[j] != index)
            dist += pow(avg_pos[j] - avg_neg[j], 2);
    }

    return sqrt(dist);
}

double Statistics::getDistCentersWithoutFeats(Data data, std::vector<int> feats, int index){
    int i = 0, j = 0, dim = data.getDim(), size = data.getSize();
    double dist = 0.0;
    int size_pos = 0, size_neg = 0, featsize = feats.size();
    vector<int> fnames = data.getFeaturesNames();
    vector<double> avg_pos(dim, 0.0), avg_neg(dim, 0.0);
    vector<shared_ptr<Point> > points = data.getPoints();

    for(size_pos = 0, size_neg = 0, i = 0; i < size; ++i){
        if(points[i]->y == 1)	size_pos++;
        else					size_neg++;
    }

    for(j = 0; j < dim; ++j){
        for(i = 0; i < size; ++i){
            if(points[i]->y == 1)
                avg_pos[j] += points[i]->x[j];
            else
                avg_neg[j] += points[i]->x[j];
        }

        avg_pos[j] /= (double) size_pos;
        avg_neg[j] /= (double) size_neg;
    }

    for(dist = 0.0, j = 0; j < dim; ++j){
        for(i = 0; i < featsize; ++i){
            if(fnames[j] == feats[i])
                dist -= pow(avg_pos[j] - avg_neg[j], 2);
        }
    }

    return sqrt(fabs(dist));
}
