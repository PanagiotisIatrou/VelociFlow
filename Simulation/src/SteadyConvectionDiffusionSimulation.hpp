#pragma once

#include "Simulation/ConvectionDiffusion.hpp"

class SteadyConvectionDiffusionSimulation : public ConvectionDiffusion {
public:
    SteadyConvectionDiffusionSimulation(Mesh *mesh, double viscosity, double tolerance_x, double tolerance_y,
                                        std::string output_file, VerboseType verbose_type);

    void solve() override;
};
