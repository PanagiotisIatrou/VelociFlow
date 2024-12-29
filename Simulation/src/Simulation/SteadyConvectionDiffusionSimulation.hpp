#pragma once

#include "Simulation/Simulation.hpp"

class SteadyConvectionDiffusionSimulation : public Simulation {
public:
    SteadyConvectionDiffusionSimulation(Mesh *mesh, double tolerance_velocity_x, double tolerance_velocity_y,
                                        std::string output_file, VerboseType verbose_type);

    void solve() override;
};
