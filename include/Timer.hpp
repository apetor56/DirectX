#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <cmath>

using namespace std::chrono;

class Timer {
public:
    Timer();

    float peek();
private:
    steady_clock::time_point first;
};

#endif // TIMER_HPP