#pragma once

#include "Simulation/Simulation.hpp"

class UnsteadySimulation : public Simulation {
private:
    double m_dt;
    int m_timesteps;

public:
    UnsteadySimulation(Mesh *mesh, double dt, int timesteps, double tolerance_velocity_x, double tolerance_velocity_y,
                       double pressure_tolerance, std::string output_file, VerboseType verbose_type);

    void solve() override;
};
