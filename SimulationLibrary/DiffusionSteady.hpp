#pragma once

#include "Simulation/DiffusionSimulation.hpp"

class DiffusionSteady : public DiffusionSimulation {
public:
    DiffusionSteady(Mesh *mesh, double viscosity, double tolerance_x, double tolerance_y, std::string output_file,
                    VerbosityType verbosity_type);

    void solve() override;
};
