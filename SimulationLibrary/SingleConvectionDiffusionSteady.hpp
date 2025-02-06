#pragma once

#include "Simulation/SingleConvectionDiffusionSimulation.hpp"

class SingleConvectionDiffusionSteady : public SingleConvectionDiffusionSimulation {
public:
    SingleConvectionDiffusionSteady(Mesh *mesh, double viscosity, double tolerance, std::string output_file);

    void solve() override;
};
