#pragma once

#include "Simulation/ConvectionDiffusionSimulation.hpp"

class ConvectionDiffusionUnsteady : public ConvectionDiffusionSimulation {
private:
    double m_dt;
    int m_timesteps;
    int m_reached_timesteps;

public:
    ConvectionDiffusionUnsteady(Mesh *mesh, double viscosity, double dt, int timesteps, double tolerance_x,
                                double tolerance_y, std::string output_file, VerbosityType verbosity_type);

    void solve() override;

    int get_reached_timesteps() const;
};
