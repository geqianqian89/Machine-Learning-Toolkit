#include "../includes/Visualisation.hpp"
#include <cstdio>

using namespace std;

Visualisation::Visualisation(){
}

Visualisation::Visualisation(Data *sample){
    samples = sample;
}

void Visualisation::setTitle(string title){
    g.set_title(title);
}

void Visualisation::setStyle(string style){
    g.set_style(style);
}

void Visualisation::createPosNegTemps(){
    int i, j, size = samples->getSize(), dim = samples->getDim();
    ofstream neg_file("temp/neg.plt"), pos_file("temp/pos.plt"), und_file("temp/und.plt");

    for(i = 0; i < size; i++){
        if(samples->getPoint(i).y == 1){
            for(j = 0; j < dim-1; j++){
                pos_file << samples->getPoint(i).x[j] << " ";
            }
            pos_file << samples->getPoint(i).x[j] << endl;
        }else if(samples->getPoint(i).y == -1){
            for(j = 0; j < dim-1; j++){
                neg_file << samples->getPoint(i).x[j] << " ";
            }
            neg_file << samples->getPoint(i).x[j] << endl;
        }else{
            for(j = 0; j < dim-1; j++){
                und_file << samples->getPoint(i).x[j] << " ";
            }
            und_file << samples->getPoint(i).x[j] << endl;
        }
    }
    pos_file.close();
    neg_file.close();
    und_file.close();
}

void Visualisation::plot2D(int x, int y){
    string dims = to_string(x) + ":" + to_string(y);
    string cmd = "plot 'temp/pos.plt' using " + dims + " title '+1' with points, 'temp/neg.plt' using " + dims + " title '-1' with points";
    createPosNegTemps();
    g.cmd(cmd);

}

void Visualisation::plot3D(int x, int y, int z){
    string dims = to_string(x) + ":" + to_string(y) + ":" + to_string(z);
    string cmd = "splot 'temp/pos.plt' using " + dims + " title '+1' with points, 'temp/neg.plt' using " + dims + " title '-1' with points";
    createPosNegTemps();
    g.cmd(cmd);
}

Visualisation::~Visualisation(){
    g.cmd("quit");
    remove("temp/tmp.plt");
    remove("temp/neg.plt");
    remove("temp/pos.plt");
    remove("temp/und.plt");
}


