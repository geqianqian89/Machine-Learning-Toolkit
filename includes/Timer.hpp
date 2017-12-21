#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <chrono>

namespace Timer{
  std::chrono::steady_clock::time_point start_time;
  std::chrono::steady_clock::time_point end_time;

  void start(){ 
  	start_time = std::chrono::steady_clock::now(); 
  }
  
  double count(){ 
  	return std::chrono::duration_cast<std::chrono::duration<double, std::chrono::seconds::period>>(end_time - start_time).count(); 
  }
  
  double end(){ 
  	end_time = std::chrono::steady_clock::now(); return count();
  }
}

#endif
