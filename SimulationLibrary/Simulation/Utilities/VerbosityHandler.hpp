#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

enum class VerbosityType { None, Percentages, Residuals };

struct Monitor {
    std::string name;
    std::function<double()> residual_getter;
    double tolerance;
    double initial_residual;
};

class VerbosityHandler {
private:
    VerbosityType m_verbosity_type;

    std::vector<Monitor> m_monitors;

    bool m_print_iterations = false;

    bool m_print_timesteps = false;

    int m_iterations_count = 0;

    int m_timesteps_count = 0;

    int m_total_timesteps;

    void print_residuals();

    void print_percentages();

public:
    VerbosityHandler(VerbosityType verbosity_type);

    VerbosityType get_verbosity_type() const;

    void add_monitor(std::string name, const std::function<double()>& residual_getter, double tolerance);

    void enable_print_iterations();

    void enable_print_timesteps(int total_timesteps);

    void set_iterations_count(int iterations_count);

    void set_timesteps_count(int timesteps_count);

    void print();
};
