#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

/*! Timer class
   \file Timer.hpp
   \author Mateus Coutinho Marim
*/

#include <chrono>

class Timer {
public:
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::milliseconds ms;

    inline void Reset(){ epoch = Clock::now(); }
    inline double Elapsed() const { return std::chrono::duration_cast<ms>(Clock::now() - epoch).count(); }
private:
    Clock::time_point epoch;
};
#endif
