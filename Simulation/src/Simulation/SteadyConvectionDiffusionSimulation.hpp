#pragma once

#include "Simulation/Simulation.hpp"

class SteadyConvectionDiffusionSimulation : public Simulation {
public:
    SteadyConvectionDiffusionSimulation(Mesh *mesh, double velocity_u_tolerance, double velocity_v_tolerance,
                                        std::string output_file, VerboseType verbose_type);

    void solve() override;
};
