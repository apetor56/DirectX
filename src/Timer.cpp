#include "Timer.hpp"


Timer::Timer() {
    first = steady_clock::now();
}

float Timer::peek() {
    auto now = steady_clock::now();
    const duration<float> elapsed = now - first;
    return elapsed.count();
}