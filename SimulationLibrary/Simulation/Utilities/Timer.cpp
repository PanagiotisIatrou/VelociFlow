#include "Timer.hpp"

#include <chrono>

void Timer::start_timer() {
    m_start_time = std::chrono::high_resolution_clock::now();
}

double Timer::get_elapsed_time() const {
    return static_cast<double>((std::chrono::high_resolution_clock::now() - m_start_time) /
                               std::chrono::milliseconds(1)) /
           1000.0;
}
