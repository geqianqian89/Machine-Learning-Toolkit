#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

/*! Timer class
   \file Timer.hpp
   \author Mateus Coutinho Marim
*/

#include <chrono>

class Timer{
private:
    std::chrono::system_clock::time_point m_init_time;
    std::chrono::system_clock::time_point m_end_time;
    long int m_max_time;
public:
    void start(){
        m_init_time = std::chrono::system_clock::now();
    }

    double end(){
        m_end_time = std::chrono::system_clock::now(); return count();
    }

    double count(){
        return std::chrono::duration_cast<std::chrono::duration<double, std::chrono::milliseconds::period>>(m_end_time - m_init_time).count();
    }

    double getDeltaTime(){
        return count() - m_max_time;
    }

    void setMaxTime(long int max_time){
        m_max_time = max_time;
    }
};

#endif
