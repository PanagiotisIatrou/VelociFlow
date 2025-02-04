#pragma once

#include "Simulation/DiffusionSimulation.hpp"

class DiffusionSteady : public DiffusionSimulation {
public:
    DiffusionSteady(Mesh *mesh, double viscosity, double tolerance, std::string output_file);

    void solve() override;
};
