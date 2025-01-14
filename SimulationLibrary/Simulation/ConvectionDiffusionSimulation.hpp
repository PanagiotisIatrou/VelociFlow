#pragma once

#include "Equations/Equations/ConvectionDiffusionX.hpp"
#include "Equations/Equations/ConvectionDiffusionY.hpp"
#include "Simulation.hpp"

class ConvectionDiffusionSimulation : public Simulation {
protected:
    double m_viscosity;

    std::unique_ptr<ConvectionDiffusionX> m_equation_convection_diffusion_x;
    std::unique_ptr<ConvectionDiffusionY> m_equation_convection_diffusion_y;

    double m_tolerance_x;
    double m_tolerance_y;

public:
    ConvectionDiffusionSimulation(Mesh* mesh, double viscosity, double tolerance_x, double tolerance_y,
                                  std::string output_file, SimulationType simulation_type,
                                  VerbosityType verbosity_type);

    void solve() override = 0;

    void iterate();
};
