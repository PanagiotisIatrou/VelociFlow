#pragma once

#include <optional>
#include <string>

#include "Simulation/Simulation.hpp"

class SimulatorContinuation {
public:
    std::string filename;
    int timesteps = 0;
    std::optional<SimulationName> simulation_name;
    std::optional<int> grid_size_x;
    std::optional<int> grid_size_y;
    double execution_time = 0.0;
    std::optional<double> density;
    std::optional<double> viscosity;
    std::optional<double> dt;
    std::map<EquationType, double> tolerances;
    std::map<EquationType, double> normalizations;
    std::optional<double> mass_normalization;
    std::vector<std::vector<double>> velocity_x;
    std::vector<std::vector<double>> velocity_y;
    std::vector<std::vector<double>> pressure;
    std::vector<std::vector<double>> dye;

    SimulatorContinuation(std::string filename);
};
