#pragma once
#include <chrono>

class Timer {
    std::chrono::time_point<std::chrono::system_clock> m_start_time;
public:
    void start_timer();
    double get_elapsed_time() const;
};
