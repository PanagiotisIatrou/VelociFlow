#include "VerbosityHandler.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

VerbosityHandler::VerbosityHandler(const VerbosityType verbosity_type) {
    m_verbosity_type = verbosity_type;
}

VerbosityType VerbosityHandler::get_verbosity_type() const {
    return m_verbosity_type;
}

void VerbosityHandler::print() {
    // Update the initial residuals
    if (m_iterations_count < 5) {
        for (Monitor &monitor : m_monitors) {
            monitor.initial_residual = monitor.residual_getter();
        }
    }

    // Print the residuals or the percentages
    switch (m_verbosity_type) {
        case VerbosityType::None: {
            break;
        }
        case VerbosityType::Percentages: {
            print_percentages();
            break;
        }
        case VerbosityType::Residuals: {
            print_residuals();
            break;
        }
        default: {
            std::cerr << "Unknown verbosity type" << std::endl;
            exit(1);
        }
    }
}

void VerbosityHandler::add_monitor(const std::string name, const std::function<double()>& residual_getter,
                                   const double tolerance) {
    const Monitor monitor = {name, residual_getter, tolerance, 0.0};
    m_monitors.push_back(monitor);
}

void VerbosityHandler::enable_print_iterations() {
    m_print_iterations = true;
}

void VerbosityHandler::enable_print_timesteps(const int total_timesteps) {
    m_print_timesteps = true;
        m_total_timesteps = total_timesteps;
}

void VerbosityHandler::set_iterations_count(const int iterations_count) {
    m_iterations_count = iterations_count;
}

void VerbosityHandler::set_timesteps_count(const int timesteps_count) {
    m_timesteps_count = timesteps_count;
}

void VerbosityHandler::print_residuals() {
    if (m_print_timesteps) {
        printf("Timesteps: %6d / %d   ", m_timesteps_count, m_total_timesteps);
    }
    if (m_print_iterations) {
        printf("Iterations: %-6d   ", m_iterations_count);
    }
    printf("[");
    for (const Monitor &monitor : m_monitors) {
        const double residual = monitor.residual_getter();
        if (residual > monitor.tolerance) {
            printf("%s: %4e   ", monitor.name.c_str(), residual);
        }
    }
    printf("]\n");
}

void VerbosityHandler::print_percentages() {
    printf("\r");
    if (m_print_timesteps) {
        printf("Timesteps: %6d / %d   ", m_timesteps_count, m_total_timesteps);
    }
    if (m_print_iterations) {
        printf("Iterations: %-6d   ", m_iterations_count);
    }
    printf("[");
    for (const Monitor &monitor : m_monitors) {
        const double initial_residual = monitor.initial_residual;
        const double residual = monitor.residual_getter();
        const double tolerance = monitor.tolerance;
        double scale;
        if (initial_residual <= tolerance) {
            scale = (std::log10(tolerance) - std::log10(residual / tolerance)) / std::log10(tolerance);
        } else {
            scale = std::log10(initial_residual / residual) / std::log10(initial_residual / tolerance);
        }
        scale = std::clamp(scale, 0.0, 1.0);
        const int percentage = static_cast<int>(std::floor(scale * 100.0));
        printf("%s: %-3d%% ", monitor.name.c_str(), percentage);
    }
    printf("]");
    std::cout << std::flush;
}
