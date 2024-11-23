#pragma once

#include <array>

#include "DiscretizationSchemes.hpp"

enum class CoefficientType;
class Node;

class MomentumCoefficients {
private:
    Node *m_node;

    std::array<double, 6> m_momentum_u_coefficients = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    std::array<double, 6> m_momentum_v_coefficients = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    std::unique_ptr<DiscretizationSchemes> m_discretization_schemes;

    std::array<double, 6> get_diffusion_effects(VelocityComponent velocity_component) const;

    std::array<double, 6> get_convection_effects(VelocityComponent velocity_component) const;

    std::array<double, 6> get_pressure_effects(VelocityComponent velocity_component) const;

    std::array<double, 6> get_time_effects(VelocityComponent velocity_component) const;

public:
    MomentumCoefficients(Node *node);

    void calculate_momentum_coefficients(VelocityComponent velocity_component, SimulationType simulation_type);

    double get_momentum_coefficient(CoefficientType type, VelocityComponent velocity_component) const;

    std::array<double, 6> get_momentum_coefficients(VelocityComponent velocity_component) const;
};
