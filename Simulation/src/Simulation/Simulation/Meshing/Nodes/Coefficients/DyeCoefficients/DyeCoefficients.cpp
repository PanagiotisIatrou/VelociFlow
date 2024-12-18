#include "DyeCoefficients.hpp"

#include "../../Node.hpp"

DyeCoefficients::DyeCoefficients(Node *node) {
    m_node = node;

    m_diffusion_coefficients = std::make_unique<DiffusionCoefficients>(node);
    m_convection_coefficients = std::make_unique<ConvectionCoefficients>(node, false);
    m_time_coefficients = std::make_unique<TimeCoefficients>(node, false);
}

void DyeCoefficients::calculate_coefficients(SimulationType simulation_type) {
    // Diffusion
    const Coefficients diffusion_coefficients = m_diffusion_coefficients->get_diffusion_effects(
        Field::Dye, DiffusionSchemes::CentralDifferencing);

    // Convection
    const Coefficients convection_coefficients = m_convection_coefficients->get_convection_effects(
        Field::Dye, ConvectionSchemes::QuickHayase);

    // Time
    Coefficients time_coefficients = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    if (simulation_type == SimulationType::Unsteady) {
        const double previous_value = m_node->get_previous_timestep_dye();
        time_coefficients = m_time_coefficients->get_time_effects(previous_value);
    }

    // Sum the coefficients
    m_coefficients = diffusion_coefficients + convection_coefficients + time_coefficients;
}

double DyeCoefficients::get_coefficient(const CoefficientType type) const {
    return m_coefficients.get_coefficient(type);
}

Coefficients DyeCoefficients::get_coefficients() const {
    return m_coefficients;
}
