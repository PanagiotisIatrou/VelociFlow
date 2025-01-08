#pragma once

#include "Simulation/Simulation.hpp"
#include "Simulation/Equations/Equations/MomentumX.hpp"
#include "Simulation/Equations/Equations/MomentumY.hpp"
#include "Simulation/Equations/Equations/PressureCorrection.hpp"
#include "Simulation/Equations/Equations/Dye.hpp"

class SteadySimulation : public Simulation {
private:
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
    SteadySimulation(Mesh *mesh, double density, double viscosity, double tolerance_momentum_x, double tolerance_momentum_y, double tolerance_mass_imbalance,
                     std::string output_file, VerboseType verbose_type);

    void solve() override;
};
