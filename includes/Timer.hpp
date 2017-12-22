#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <chrono>

class Timer{
private:
  std::chrono::steady_clock::time_point m_init_time;
  std::chrono::steady_clock::time_point m_end_time;

public:
  void start(){
  	m_init_time = std::chrono::steady_clock::now();
  }

  double count(){
  	return std::chrono::duration_cast<std::chrono::duration<double, std::chrono::seconds::period>>(m_end_time - m_init_time).count();
  }

  double end(){
  	m_end_time = std::chrono::steady_clock::now(); return count();
  }
};

#endif
