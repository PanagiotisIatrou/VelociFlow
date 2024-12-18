#include "TimeCoefficients.hpp"

#include "../../Node.hpp"

TimeCoefficients::TimeCoefficients(Node *node, const bool include_density) {
    m_node = node;
    m_include_density = include_density;
}

Coefficients TimeCoefficients::get_time_effects(const VelocityComponent velocity_component) const {
    Coefficients coefficients;

    double extra = m_node->get_dx() * m_node->get_dy();
    if (m_include_density) {
        extra *= m_node->get_density();
    }

    coefficients.center += extra;

    if (velocity_component == VelocityComponent::U) {
        extra *= m_node->get_previous_timestep_velocity_u();
    } else {
        extra *= m_node->get_previous_timestep_velocity_v();
    }
    coefficients.source += extra;

    return coefficients;
}
