#pragma once

#include <memory>

#include "Equations/Equations/DiffusionX.hpp"
#include "Equations/Equations/DiffusionY.hpp"
#include "Simulation.hpp"

class DiffusionSimulation : public Simulation {
protected:
    double m_viscosity;

    std::unique_ptr<DiffusionX> m_equation_diffusion_x;
    std::unique_ptr<DiffusionY> m_equation_diffusion_y;

    double m_tolerance_x;
    double m_tolerance_y;

public:
    DiffusionSimulation(Mesh* mesh, double viscosity, double tolerance_x, double tolerance_y, std::string output_file,
                        SimulationType simulation_type, VerbosityType verbosity_type);

    void solve() override = 0;

    void iterate();
};
