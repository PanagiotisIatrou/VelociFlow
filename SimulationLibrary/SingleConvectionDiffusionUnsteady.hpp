#pragma once

#include "Simulation/SingleConvectionDiffusionSimulation.hpp"

class SingleConvectionDiffusionUnsteady : public SingleConvectionDiffusionSimulation {
private:
    double m_dt;
    int m_timesteps;
    int m_reached_timesteps;

public:
    SingleConvectionDiffusionUnsteady(Mesh *mesh, double viscosity, double dt, int timesteps, double tolerance,
                                      std::string output_file);

    void solve() override;

    int get_reached_timesteps() const;
};
