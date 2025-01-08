#pragma once

#include "Simulation/ConvectionDiffusion.hpp"

class ConvectionDiffusionSteady : public ConvectionDiffusion {
public:
    ConvectionDiffusionSteady(Mesh *mesh, double viscosity, double tolerance_x, double tolerance_y,
                                        std::string output_file, VerboseType verbose_type);

    void solve() override;
};
