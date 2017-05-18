#include <iostream>
#include <fstream>
#include <unistd.h>
#include <ios>
#include <numeric>
#include "../includes/MLToolkit.hpp"

using namespace std;

void process_mem_usage(double& vm_usage, double& resident_set){
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}

int main(int argc, char *argv[])
{
    Data data, cp;
    int i;
    Point p, q;
    vector<double> o(4);
    vector<int> fnames, feats(2);
    double a, b;

    data.load(string(argv[1]));
    cp.load(string(argv[1]));


    process_mem_usage(a, b);
    cout << a << " " << b << endl;
    o[0] = 0.1, o[1] = 1, o[2] = 2, o[3] = 3;
    p.x = o;
    p.y = -1;
    q = data.getPoint(1);
    data.normalize();
    for(Point p: data.getPoints()){
        cout << p << endl;
    }

    Data::normalize(q.x, 2);
    cout << q << endl;
    fnames = data.getFeaturesNames();
    iota(feats.begin(), feats.end(), 1);
    for(int i = 0; i < fnames.size(); i++) cout << fnames[i] << endl;
    cp = data.copy();
    cout << "Dataset radius: " << Statistics::getRadius(cp, -1, 2) << endl;
    cout << "Dataset dist centers: " << Statistics::getDistCenters(cp, -1) << endl;
    cout << "Dataset dist centers*: " << Statistics::getDistCentersWithoutFeats(cp, feats, -1) << endl;
    cout << "Dataset size: " << cp.getSize() << endl;
    cout << "Dataset dim: " << cp.getDim() << endl;
    cout << "Negative Points: " << cp.getNumberNegativePoints() << endl;
    cout << "Positive Points: " << cp.getNumberPositivePoints() << endl;
	cout << endl;
	cout << "Dataset size: " << data.getSize() << endl;
    cout << "Dataset dim: " << data.getDim() << endl;
    cout << "Negative Points: " << data.getNumberNegativePoints() << endl;
    cout << "Positive Points: " << data.getNumberPositivePoints() << endl;
    
    return 0;
}
