#pragma once

#include <memory>

#include "Discretization/Equations/SingleConvectionDiffusionEquation.hpp"
#include "Simulation.hpp"

class SingleConvectionDiffusionSimulation : public Simulation {
protected:
    double m_viscosity;

    std::unique_ptr<SingleConvectionDiffusionEquation> m_equation_single_convection_diffusion;

public:
    SingleConvectionDiffusionSimulation(Mesh* mesh, double viscosity, double tolerance, std::string output_file,
                                        SimulationType simulation_type);

    void solve() override = 0;

    void iterate();
};
