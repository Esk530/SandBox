#ifndef __PO_TIMER_H__
#define __PO_TIMER_H__

#include <iostream>
#include <chrono>
#include <string>
namespace po
{
    class simpleTimer {
    public:
        simpleTimer(std::string _name) : m_start_time(std::chrono::system_clock::now()),name(_name) {}
        ~simpleTimer(){
            std::cout << "Elapsed time(" << name  << ")" << std::endl;
            auto end = std::chrono::system_clock::now();
            double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-m_start_time).count();
            std::cout << elapsed << " ms" << std::endl;
        }
    private:
        std::string name;
        std::chrono::system_clock::time_point m_start_time;
    };
}

#endif// __PO_TIMER_H__