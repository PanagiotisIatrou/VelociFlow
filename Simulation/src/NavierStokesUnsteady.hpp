#pragma once

#include "Simulation/NavierStokes.hpp"

class NavierStokesUnsteady : public NavierStokes {
private:
    double m_dt;
    int m_timesteps;

public:
    NavierStokesUnsteady(Mesh *mesh, double density, double viscosity, double dt, int timesteps,
                       double tolerance_momentum_x, double tolerance_momentum_y, double tolerance_mass_imbalance,
                       std::string output_file, VerboseType verbose_type);

    void solve() override;
};
