#pragma once

#include "Discretization/Equations/ConvectionDiffusionXEquation.hpp"
#include "Discretization/Equations/ConvectionDiffusionYEquation.hpp"
#include "Simulation.hpp"

class ConvectionDiffusionSimulation : public Simulation {
protected:
    double m_viscosity;

    std::unique_ptr<ConvectionDiffusionXEquation> m_equation_convection_diffusion_x;
    std::unique_ptr<ConvectionDiffusionYEquation> m_equation_convection_diffusion_y;

    double m_tolerance_x;
    double m_tolerance_y;

public:
    ConvectionDiffusionSimulation(Mesh* mesh, double viscosity, double tolerance_x, double tolerance_y,
                                  std::string output_file, SimulationType simulation_type,
                                  VerbosityType verbosity_type);

    void solve() override = 0;

    void iterate();
};
