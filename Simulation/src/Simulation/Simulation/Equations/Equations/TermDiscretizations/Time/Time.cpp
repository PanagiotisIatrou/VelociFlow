#include "Time.hpp"

#include "../../../../Meshing/Nodes/Node.hpp"

Time::Time(Node *node, const Field field, const bool include_density) : TermDiscretization(node) {
    m_field = field;
    m_include_density = include_density;
}

Coefficients Time::get_effects() const {
    Coefficients coefficients;

    double extra = m_node->get_dx() * m_node->get_dy();
    if (m_include_density) {
        extra *= m_node->get_density();
    }

    coefficients.center += extra;
    coefficients.source += extra * m_node->get_previous_timestep_field_value(m_field);

    return coefficients;
}
