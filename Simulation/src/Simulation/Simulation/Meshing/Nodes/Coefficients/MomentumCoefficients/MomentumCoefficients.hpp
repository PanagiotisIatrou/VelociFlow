#pragma once

#include "../Coefficients.hpp"

class Node;

class MomentumCoefficients {
private:
    Node *m_node;

    Coefficients m_momentum_u_coefficients;
    Coefficients m_momentum_v_coefficients;

    Coefficients get_diffusion_effects(VelocityComponent velocity_component) const;

    Coefficients get_convection_effects(VelocityComponent velocity_component) const;

    Coefficients get_pressure_effects(VelocityComponent velocity_component) const;

    Coefficients get_time_effects(VelocityComponent velocity_component) const;

public:
    MomentumCoefficients(Node *node);

    void calculate_momentum_coefficients(VelocityComponent velocity_component, SimulationType simulation_type);

    double get_momentum_coefficient(CoefficientType type, VelocityComponent velocity_component) const;

    Coefficients get_momentum_coefficients(VelocityComponent velocity_component) const;
};
