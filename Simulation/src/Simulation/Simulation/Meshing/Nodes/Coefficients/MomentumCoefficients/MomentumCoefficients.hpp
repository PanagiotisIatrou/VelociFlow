#pragma once

#include "../Coefficients.hpp"

class Node;

enum class DiffusionSchemes { CentralDifferencing };

enum class ConvectionSchemes { Upwind, CentralDifferencing, QuickHayase };

class MomentumCoefficients {
private:
    Node *m_node;

    Coefficients m_momentum_u_coefficients;
    Coefficients m_momentum_v_coefficients;

    // Diffusion

    Coefficients get_diffusion_effects(VelocityComponent velocity_component, DiffusionSchemes diffusion_scheme) const;

    Coefficients get_central_differencing_diffusion_effects(Direction direction, VelocityComponent velocity_component) const;

    // Convection

    Coefficients get_convection_effects(VelocityComponent velocity_component, ConvectionSchemes convection_scheme) const;

    Coefficients get_upwind_convection_effects(Direction direction, VelocityComponent velocity_component) const;

    Coefficients get_central_differencing_convection_effects(Direction direction, VelocityComponent velocity_component) const;

    Coefficients get_quick_hayase_convection_effects(Direction direction, VelocityComponent velocity_component) const;

    // Pressure

    Coefficients get_pressure_effects(VelocityComponent velocity_component) const;

    // Time

    Coefficients get_time_effects(VelocityComponent velocity_component) const;

public:
    MomentumCoefficients(Node *node);

    void calculate_momentum_coefficients(VelocityComponent velocity_component, SimulationType simulation_type);

    double get_momentum_coefficient(CoefficientType type, VelocityComponent velocity_component) const;

    Coefficients get_momentum_coefficients(VelocityComponent velocity_component) const;
};
