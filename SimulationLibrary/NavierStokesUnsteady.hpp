#pragma once

#include "Simulation/NavierStokesSimulation.hpp"
#include "SimulatorContinuation.hpp"

class NavierStokesUnsteady : public NavierStokesSimulation {
private:
    double m_dt;
    int m_timesteps;
    int m_reached_timesteps = 0;
    bool m_continuing_simulation = false;

public:
    NavierStokesUnsteady(Mesh *mesh, double density, double viscosity, double dt, int timesteps,
                       double tolerance_momentum_x, double tolerance_momentum_y, double tolerance_mass_imbalance,
                       std::string output_file, VerbosityType verbosity_type);

    NavierStokesUnsteady(Mesh *mesh, SimulatorContinuation *simulator_continuation, int extra_timesteps, VerbosityType verbosity_type);

    void solve() override;

    int get_reached_timesteps() const;
};
