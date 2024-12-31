#include "MomentumCoefficients.hpp"

#include "../../Node.hpp"
#include "../../../../../common.hpp"

MomentumCoefficients::MomentumCoefficients(Node *node) {
    m_node = node;

    m_convection_coefficients = std::make_unique<ConvectionCoefficients>(node, true);
    m_diffusion_coefficients = std::make_unique<DiffusionCoefficients>(node);
    m_time_coefficients = std::make_unique<TimeCoefficients>(node, true);
    m_pressure_coefficients = std::make_unique<PressureCoefficients>(node);
}

void MomentumCoefficients::calculate_coefficients(const VelocityComponent velocity_component,
                                                           const SimulationType simulation_type) {
    Field field;
    Variable variable;
    if (velocity_component == VelocityComponent::U) {
        field = Field::VelocityX;
        variable = Variable::VelocityX;
    } else {
        field = Field::VelocityY;
        variable = Variable::VelocityY;
    }

    // Diffusion
    const Coefficients diffusion_coefficients = m_diffusion_coefficients->get_diffusion_effects(
        field, DiffusionSchemes::CentralDifferencing);

    // Convection
    const Coefficients convection_coefficients = m_convection_coefficients->get_convection_effects(
        field, ConvectionSchemes::QuickHayase);

    // Pressure
    const Coefficients pressure_coefficients = m_pressure_coefficients->get_pressure_effects(velocity_component);

    // Time
    Coefficients time_coefficients = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    if (simulation_type == SimulationType::Unsteady) {
        double previous_value = m_node->get_previous_timestep_variable_value(variable);
        time_coefficients = m_time_coefficients->get_time_effects(previous_value);
    }

    // Sum the coefficients
    Coefficients coefficients = diffusion_coefficients + convection_coefficients + pressure_coefficients +
                                time_coefficients;

    if (velocity_component == VelocityComponent::U) {
        // Under-relaxation
        coefficients.center /= relaxation_velocity_x;
        coefficients.source += (1 - relaxation_velocity_x) * coefficients.center * m_node->get_field_value(Field::VelocityX);

        m_coefficients_x = coefficients;
    } else {
        // Under-relaxation
        coefficients.center /= relaxation_velocity_y;
        coefficients.source += (1 - relaxation_velocity_y) * coefficients.center * m_node->get_field_value(Field::VelocityY);

        m_coefficients_y = coefficients;
    }
}

double MomentumCoefficients::get_coefficient(const CoefficientType type,
                                                      const VelocityComponent velocity_component) const {
    if (velocity_component == VelocityComponent::U) {
        return m_coefficients_x.get_coefficient(type);
    } else {
        return m_coefficients_y.get_coefficient(type);
    }
}

Coefficients MomentumCoefficients::get_coefficients(const VelocityComponent velocity_component) const {
    if (velocity_component == VelocityComponent::U) {
        return m_coefficients_x;
    } else {
        return m_coefficients_y;
    }
}
