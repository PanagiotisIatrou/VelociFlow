#pragma once

#include "Simulation/NavierStokes.hpp"

class NavierStokesSteady : public NavierStokes {
public:
    NavierStokesSteady(Mesh *mesh, double density, double viscosity, double tolerance_momentum_x,
                     double tolerance_momentum_y, double tolerance_mass_imbalance, std::string output_file,
                     VerboseType verbose_type);

    void solve() override;
};
