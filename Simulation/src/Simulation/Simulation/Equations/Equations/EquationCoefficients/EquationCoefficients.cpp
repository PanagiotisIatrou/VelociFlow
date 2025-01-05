#include "EquationCoefficients.hpp"

#include "../../../Meshing/Nodes/Node.hpp"

EquationCoefficients::EquationCoefficients(Node *node, const Field variable_field, const double relaxation) {
    m_node = node;
    m_variable_field = variable_field;
    m_relaxation = relaxation;
}

void EquationCoefficients::calculate_coefficients() {
    m_coefficients = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    for (const auto &term_discretization : m_term_discretizations) {
        const Coefficients coefficients = term_discretization->get_effects();
        m_coefficients += coefficients;
    }

    // Apply the relaxation
    m_coefficients.center /= m_relaxation;
    m_coefficients.source += (1 - m_relaxation) * m_coefficients.center * m_node->get_field_value(m_variable_field);
}

double EquationCoefficients::get_coefficient(const CoefficientType type) const {
    return m_coefficients.get_coefficient(type);
}

Coefficients EquationCoefficients::get_coefficients() const {
    return m_coefficients;
}
