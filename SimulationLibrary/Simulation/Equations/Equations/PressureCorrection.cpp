#include "PressureCorrection.hpp"

#include <cmath>

PressureCorrection::PressureCorrection(Mesh *mesh, const Field variable_field, const double relaxation_factor,
                                       const ResidualType residual_type, const StoppingRule stopping_rule,
                                       const NormType norm_type, const double stopping_tolerance)
    : Equation(mesh, EquationType::PressureCorrection, variable_field, relaxation_factor, residual_type, stopping_rule,
               norm_type, stopping_tolerance, false) {
}

void PressureCorrection::calculate_mass_imbalance() {
    m_mass_imbalance = 0.0;
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            const Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            const double residual =
                std::abs(node_P->get_equation_coefficient(m_equation_type, CoefficientType::Source));
            m_mass_imbalance = std::max(m_mass_imbalance, residual);
        }
    }

    if (m_can_update_mass_imbalance_normalization_factor) {
        m_mass_imbalance_normalization_factor = std::max(m_mass_imbalance_normalization_factor, m_mass_imbalance);

        if (m_iterations_count >= imbalance_normalization_iterations) {
            m_can_update_mass_imbalance_normalization_factor = false;
        }
    }

    if (m_mass_imbalance_normalization_factor != 0.0) {
        m_mass_imbalance /= std::max(m_mass_imbalance_normalization_factor, 1e-10);
    }
}

double PressureCorrection::get_mass_imbalance() const {
    return m_mass_imbalance;
}

void PressureCorrection::reset_mass_imbalance() {
    m_mass_imbalance = std::numeric_limits<double>::infinity();
}

double PressureCorrection::get_mass_imbalance_normalization_factor() const {
    return m_mass_imbalance_normalization_factor;
}

void PressureCorrection::set_mass_imbalance_normalization_factor(const double factor) {
    m_mass_imbalance_normalization_factor = factor;
    m_can_update_mass_imbalance_normalization_factor = false;
}
