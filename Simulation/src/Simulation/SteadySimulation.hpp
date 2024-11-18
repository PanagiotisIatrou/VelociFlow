#pragma once

#include "Simulation/Simulation.hpp"

class SteadySimulation : public Simulation {
public:
    SteadySimulation(Mesh *mesh, double velocity_u_tolerance, double velocity_v_tolerance, double pressure_tolerance, std::string output_file);

    void solve() override;
};
