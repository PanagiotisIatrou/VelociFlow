#pragma once

#include "Simulation/NavierStokesSimulation.hpp"

class NavierStokesSteady : public NavierStokesSimulation {
public:
    NavierStokesSteady(Mesh *mesh, double density, double viscosity, double tolerance_momentum_x,
                       double tolerance_momentum_y, double tolerance_mass_imbalance, std::string output_file,
                       VerbosityType verbosity_type);

    void solve() override;
};
