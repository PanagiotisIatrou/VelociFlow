#include "PressureCoefficients.hpp"

#include "../../Node.hpp"

class Face;

PressureCoefficients::PressureCoefficients(Node *node) {
    m_node = node;
}

Coefficients PressureCoefficients::get_pressure_effects(const VelocityComponent velocity_component) const {
    Coefficients coefficients;

    if (velocity_component == VelocityComponent::U) {
        const Face *face_w = m_node->get_neighbouring_face(Direction::West);
        const Face *face_e = m_node->get_neighbouring_face(Direction::East);

        coefficients.source += m_node->get_dt() * m_node->get_dy() * (face_w->get_pressure() - face_e->get_pressure());
    } else {
        const Face *face_s = m_node->get_neighbouring_face(Direction::South);
        const Face *face_n = m_node->get_neighbouring_face(Direction::North);

        coefficients.source += m_node->get_dt() * m_node->get_dx() * (face_s->get_pressure() - face_n->get_pressure());
    }

    return coefficients;
}
