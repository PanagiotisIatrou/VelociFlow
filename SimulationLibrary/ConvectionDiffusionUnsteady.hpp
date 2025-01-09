#pragma once

#include "Simulation/ConvectionDiffusion.hpp"

class ConvectionDiffusionUnsteady : public ConvectionDiffusion {
private:
    double m_dt;
    int m_timesteps;

public:
    ConvectionDiffusionUnsteady(Mesh *mesh, double viscosity, double dt, int timesteps, double tolerance_x,
                                double tolerance_y, std::string output_file, VerbosityType verbosity_type);

    void solve() override;
};
