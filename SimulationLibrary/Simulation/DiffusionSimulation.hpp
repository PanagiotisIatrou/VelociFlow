#pragma once

#include <memory>

#include "Equations/Equations/DiffusionEquation.hpp"
#include "Simulation.hpp"

class DiffusionSimulation : public Simulation {
protected:
    double m_viscosity;

    std::unique_ptr<DiffusionEquation> m_equation_diffusion;

public:
    DiffusionSimulation(Mesh* mesh, double viscosity, double tolerance, std::string output_file,
                        SimulationType simulation_type);

    void solve() override = 0;

    void iterate();
};
