#pragma once

#include "Equations/Equations/Dye.hpp"
#include "Equations/Equations/MomentumX.hpp"
#include "Equations/Equations/MomentumY.hpp"
#include "Equations/Equations/PressureCorrection.hpp"
#include "Simulation.hpp"

class NavierStokes : public Simulation {
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
    NavierStokes(Mesh *mesh, double density, double viscosity, double tolerance_momentum_x, double tolerance_momentum_y,
                 double tolerance_mass_imbalance, std::string output_file, SimulationType simulation_type,
                 VerbosityType verbosity_type);

    void solve() override = 0;

    void simple_iterate();
};
