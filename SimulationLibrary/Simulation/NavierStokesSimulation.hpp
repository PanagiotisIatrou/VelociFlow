#pragma once

#include "Discretization/Equations/DyeEquation.hpp"
#include "Discretization/Equations/MomentumXEquation.hpp"
#include "Discretization/Equations/MomentumYEquation.hpp"
#include "Discretization/Equations/PressureCorrectionEquation.hpp"
#include "Simulation.hpp"

class NavierStokesSimulation : public Simulation {
protected:
    double m_density;
    double m_viscosity;

    std::unique_ptr<MomentumXEquation> m_equation_momentum_x;
    std::unique_ptr<MomentumYEquation> m_equation_momentum_y;
    std::unique_ptr<PressureCorrectionEquation> m_equation_pressure_correction;
    std::unique_ptr<DyeEquation> m_equation_dye;

    double m_tolerance_momentum_x;
    double m_tolerance_momentum_y;
    double m_tolerance_mass_imbalance;

public:
    NavierStokesSimulation(Mesh *mesh, double density, double viscosity, double tolerance_momentum_x,
                           double tolerance_momentum_y, double tolerance_mass_imbalance, std::string output_file,
                           SimulationType simulation_type, VerbosityType verbosity_type);

    void solve() override = 0;

    void simple_iterate();
};
