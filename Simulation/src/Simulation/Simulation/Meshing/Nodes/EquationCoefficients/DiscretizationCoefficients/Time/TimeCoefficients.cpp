#include "TimeCoefficients.hpp"

#include "../../../Node.hpp"

TimeCoefficients::TimeCoefficients(Node *node, const bool include_density) {
    m_node = node;
    m_include_density = include_density;
}

Coefficients TimeCoefficients::get_time_effects(const double previous_value) const {
    Coefficients coefficients;

    double extra = m_node->get_dx() * m_node->get_dy();
    if (m_include_density) {
        extra *= m_node->get_field_value(Field::Density);
    }

    coefficients.center += extra;
    coefficients.source += extra * previous_value;

    return coefficients;
}
