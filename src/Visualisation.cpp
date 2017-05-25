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

void Visualisation::plot2D(int x, int y){
    remove("temp/tmp.plt");
    samples->write("temp/tmp", "plt");
    g.plotfile_xy("temp/tmp.plt", x, y);
}

void Visualisation::plot3D(int x, int y, int z){
    remove("temp/tmp.plt");
    samples->write("temp/tmp", "plt");
    g.plotfile_xyz("temp/tmp.plt", x, y, z);
}

Visualisation::~Visualisation(){
    g.cmd("quit");
    remove("temp/tmp.plt");
}


