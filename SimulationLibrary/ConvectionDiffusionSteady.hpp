#pragma once

#include "Simulation/ConvectionDiffusionSimulation.hpp"

class ConvectionDiffusionSteady : public ConvectionDiffusionSimulation {
public:
    ConvectionDiffusionSteady(Mesh *mesh, double viscosity, double tolerance_x, double tolerance_y,
                                        std::string output_file, VerbosityType verbosity_type);

    void solve() override;
};
