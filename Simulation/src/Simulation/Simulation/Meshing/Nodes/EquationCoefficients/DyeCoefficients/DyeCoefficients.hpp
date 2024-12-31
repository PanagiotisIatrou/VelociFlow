#pragma once

#include <memory>

#include "../../Coefficients/Coefficients.hpp"
#include "../../Coefficients/Convection/ConvectionCoefficients.hpp"
#include "../../Coefficients/Diffusion/DiffusionCoefficients.hpp"
#include "../../Coefficients/Time/TimeCoefficients.hpp"

class DyeCoefficients {
private:
    Node *m_node;

    Coefficients m_coefficients;

    // Discretization coefficients

    std::unique_ptr<DiffusionCoefficients> m_diffusion_coefficients;
    std::unique_ptr<ConvectionCoefficients> m_convection_coefficients;
    std::unique_ptr<TimeCoefficients> m_time_coefficients;

public:
    DyeCoefficients(Node *node);

    void calculate_coefficients(SimulationType simulation_type);

    double get_coefficient(CoefficientType type) const;

    Coefficients get_coefficients() const;
};
