#include <iostream>
#include <fstream>
#include <unistd.h>
#include <ios>
#include <numeric>
#include "../includes/Data.hpp"

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
    double a, b;
    data.load(string(argv[1]));

    cout << "Dataset size: " << data.getSize() << endl;
    cout << "Dataset dim: " << data.getDim() << endl;
    cout << "Negative Points: " << data.getNumberNegativePoints() << endl;
    cout << "Positive Points: " << data.getNumberPositivePoints() << endl;
    process_mem_usage(a, b);
    cout << a << " " << b << endl;

    vector<int> ids(20);

    iota(ids.begin(), ids.end(), 21);

    cp = data;
    cp.removePoints(ids);

    for(Point p: cp.points){
        cout << p.id << "- ";
        for(int i = 0; i < p.x.size(); i++){
            cout << p.x[i] << " ";
        }
        cout << endl;
    }
    cout << "Dataset size: " << cp.getSize() << endl;
    cout << "Dataset dim: " << cp.getDim() << endl;
    cout << "Negative Points: " << cp.getNumberNegativePoints() << endl;
    cout << "Positive Points: " << cp.getNumberPositivePoints() << endl;
    return 0;
}
