#pragma once

#include <memory>

#include "DiscretizationCoefficients/Coefficients.hpp"
#include "DiscretizationCoefficients/Convection/ConvectionCoefficients.hpp"
#include "DiscretizationCoefficients/Diffusion/DiffusionCoefficients.hpp"
#include "DiscretizationCoefficients/Time/TimeCoefficients.hpp"
#include "DiscretizationCoefficients/Pressure/PressureCoefficients.hpp"

class MomentumCoefficients {
private:
    Node *m_node;

    Coefficients m_coefficients_x;
    Coefficients m_coefficients_y;

    // Discretization coefficients

    std::unique_ptr<DiffusionCoefficients> m_diffusion_coefficients;
    std::unique_ptr<ConvectionCoefficients> m_convection_coefficients;
    std::unique_ptr<PressureCoefficients> m_pressure_coefficients;
    std::unique_ptr<TimeCoefficients> m_time_coefficients;

public:
    MomentumCoefficients(Node *node);

    void calculate_coefficients(VelocityComponent velocity_component, SimulationType simulation_type);

    double get_coefficient(CoefficientType type, VelocityComponent velocity_component) const;

    Coefficients get_coefficients(VelocityComponent velocity_component) const;
};
