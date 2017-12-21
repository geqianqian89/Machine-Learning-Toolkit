#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <chrono>

class Timer{
private:
  std::chrono::steady_clock::time_point start_time;
  std::chrono::steady_clock::time_point end_time;

public:
  Timer(){}
  inline void start(){ start_time = std::chrono::steady_clock::now(); }
  inline double count(){ return std::chrono::duration_cast<std::chrono::duration<double, std::chrono::seconds::period>>(end_time - start_time).count(); }
  inline double end(){ end_time = std::chrono::steady_clock::now(); return count();}
};

#endif
