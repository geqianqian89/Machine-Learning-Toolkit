#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

/*! Timer class
   \file Timer.hpp
   \author Mateus Coutinho Marim
*/

#include <cstdio>
#include <ctime>

class Timer {
public:
    inline Timer() {start = std::clock();}
    inline void Reset(){ start = std::clock(); }
    inline double Elapsed() const { return 100.0f*( std::clock() - start ) / (double) CLOCKS_PER_SEC; }
private:
    std::clock_t start;
    double duration;
};
#endif
