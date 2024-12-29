#pragma once

#include "Simulation/Simulation.hpp"

class SteadySimulation : public Simulation {
public:
    SteadySimulation(Mesh *mesh, double tolerance_velocity_x, double tolerance_velocity_y, double pressure_tolerance,
                     std::string output_file, VerboseType verbose_type);

    void solve() override;
};
