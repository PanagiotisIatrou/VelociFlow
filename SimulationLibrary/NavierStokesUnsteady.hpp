#pragma once

#include "Simulation/NavierStokesSimulation.hpp"

class NavierStokesUnsteady : public NavierStokesSimulation {
private:
    double m_dt;
    int m_timesteps;

public:
    NavierStokesUnsteady(Mesh *mesh, double density, double viscosity, double dt, int timesteps,
                       double tolerance_momentum_x, double tolerance_momentum_y, double tolerance_mass_imbalance,
                       std::string output_file, VerbosityType verbosity_type);

    void solve() override;
};
