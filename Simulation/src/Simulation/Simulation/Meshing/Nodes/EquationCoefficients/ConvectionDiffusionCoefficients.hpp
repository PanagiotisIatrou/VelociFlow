#pragma once

#include <memory>

#include "DiscretizationCoefficients/Coefficients.hpp"
#include "DiscretizationCoefficients/Convection/ConvectionCoefficients.hpp"
#include "DiscretizationCoefficients/Diffusion/DiffusionCoefficients.hpp"
#include "DiscretizationCoefficients/Time/TimeCoefficients.hpp"

class ConvectionDiffusionCoefficients {
private:
    Node *m_node;

    Coefficients m_coefficients_x;
    Coefficients m_coefficients_y;

    // Discretization coefficients

    std::unique_ptr<DiffusionCoefficients> m_diffusion_coefficients;
    std::unique_ptr<ConvectionCoefficients> m_convection_coefficients;
    std::unique_ptr<TimeCoefficients> m_time_coefficients;

public:
    ConvectionDiffusionCoefficients(Node *node);

    void calculate_coefficients(SimulationType simulation_type, VelocityComponent velocity_component);

    double get_coefficient(CoefficientType type, VelocityComponent velocity_component) const;

    Coefficients get_coefficients(VelocityComponent velocity_component) const;
};
