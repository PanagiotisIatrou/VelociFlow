#pragma once

#include "Simulation/Simulation.hpp"

class UnsteadySimulation : public Simulation {
private:
    double m_dt;
    int m_timesteps;

public:
    UnsteadySimulation(Mesh *mesh, double dt, int timesteps, double velocity_u_tolerance, double velocity_v_tolerance,
                       double pressure_tolerance, std::string output_file, VerboseType verbose_type);

    void solve() override;
};
