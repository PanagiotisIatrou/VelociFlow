#include "ConvectionDiffusionCoefficients.hpp"

#include "../../Node.hpp"

ConvectionDiffusionCoefficients::ConvectionDiffusionCoefficients(Node *node) {
    m_node = node;

    m_diffusion_coefficients = std::make_unique<DiffusionCoefficients>(node);
    m_convection_coefficients = std::make_unique<ConvectionCoefficients>(node, false);
    m_time_coefficients = std::make_unique<TimeCoefficients>(node, false);
}

void ConvectionDiffusionCoefficients::calculate_coefficients(const SimulationType simulation_type,
                                                             const VelocityComponent velocity_component) {
    Field field;
    if (velocity_component == VelocityComponent::U) {
        field = Field::VelocityX;
    } else {
        field = Field::VelocityY;
    }

    // Diffusion
    const Coefficients diffusion_coefficients = m_diffusion_coefficients->get_diffusion_effects(
        field, DiffusionSchemes::CentralDifferencing);

    // Convection
    const Coefficients convection_coefficients = m_convection_coefficients->get_convection_effects(
        field, ConvectionSchemes::QuickHayase);

    // Time
    Coefficients time_coefficients = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    if (simulation_type == SimulationType::Unsteady) {
        double previous_value;
        if (velocity_component == VelocityComponent::U) {
            previous_value = m_node->get_previous_timestep_velocity_u();
        } else {
            previous_value = m_node->get_previous_timestep_velocity_v();
        }
        time_coefficients = m_time_coefficients->get_time_effects(previous_value);
    }

    // Sum the coefficients
    if (velocity_component == VelocityComponent::U) {
        m_coefficients_x = diffusion_coefficients + convection_coefficients + time_coefficients;
    } else {
        m_coefficients_y = diffusion_coefficients + convection_coefficients + time_coefficients;
    }
}

double ConvectionDiffusionCoefficients::get_coefficient(const CoefficientType type,
                                                        const VelocityComponent velocity_component) const {
    if (velocity_component == VelocityComponent::U) {
        return m_coefficients_x.get_coefficient(type);
    } else {
        return m_coefficients_y.get_coefficient(type);
    }
}

Coefficients ConvectionDiffusionCoefficients::get_coefficients(const VelocityComponent velocity_component) const {
    if (velocity_component == VelocityComponent::U) {
        return m_coefficients_x;
    } else {
        return m_coefficients_y;
    }
}
