#pragma once

#include <memory>

#include "../Coefficients.hpp"
#include "../Convection/ConvectionCoefficients.hpp"
#include "../Diffusion/DiffusionCoefficients.hpp"
#include "../Time/TimeCoefficients.hpp"
#include "../Pressure/PressureCoefficients.hpp"

class MomentumCoefficients {
private:
    Node *m_node;

    Coefficients m_momentum_u_coefficients;
    Coefficients m_momentum_v_coefficients;

    // Discretization coefficients

    std::unique_ptr<DiffusionCoefficients> m_diffusion_coefficients;
    std::unique_ptr<ConvectionCoefficients> m_convection_coefficients;
    std::unique_ptr<PressureCoefficients> m_pressure_coefficients;
    std::unique_ptr<TimeCoefficients> m_time_coefficients;

public:
    MomentumCoefficients(Node *node);

    void calculate_momentum_coefficients(VelocityComponent velocity_component, SimulationType simulation_type);

    double get_momentum_coefficient(CoefficientType type, VelocityComponent velocity_component) const;

    Coefficients get_momentum_coefficients(VelocityComponent velocity_component) const;
};
