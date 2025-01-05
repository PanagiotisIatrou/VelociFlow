#pragma once

#include "Simulation/Simulation.hpp"
#include "Simulation/Equations/Equations/ConvectionDiffusionX.hpp"
#include "Simulation/Equations/Equations/ConvectionDiffusionY.hpp"

class SteadyConvectionDiffusionSimulation : public Simulation {
private:
    std::unique_ptr<ConvectionDiffusionX> m_equation_convection_diffusion_x;
    std::unique_ptr<ConvectionDiffusionY> m_equation_convection_diffusion_y;

    double m_tolerance_x;
    double m_tolerance_y;

public:
    SteadyConvectionDiffusionSimulation(Mesh *mesh, double tolerance_x, double tolerance_y,
                                        std::string output_file, VerboseType verbose_type);

    void solve() override;
};
