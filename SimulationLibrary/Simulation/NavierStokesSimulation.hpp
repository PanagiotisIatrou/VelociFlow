#pragma once

#include "Discretization/Equations/Dye.hpp"
#include "Discretization/Equations/MomentumX.hpp"
#include "Discretization/Equations/MomentumY.hpp"
#include "Discretization/Equations/PressureCorrection.hpp"
#include "Simulation.hpp"

class NavierStokesSimulation : public Simulation {
protected:
    double m_density;
    double m_viscosity;

    std::unique_ptr<MomentumX> m_equation_momentum_x;
    std::unique_ptr<MomentumY> m_equation_momentum_y;
    std::unique_ptr<PressureCorrection> m_equation_pressure_correction;
    std::unique_ptr<Dye> m_equation_dye;

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
