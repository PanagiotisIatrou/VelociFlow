#include "PressureCoefficients.hpp"

#include "../../../Node.hpp"

class Face;

PressureCoefficients::PressureCoefficients(Node *node) {
    m_node = node;
}

Coefficients PressureCoefficients::get_pressure_effects(const VelocityComponent velocity_component) const {
    Coefficients coefficients;

    if (velocity_component == VelocityComponent::U) {
        const Face *face_w = m_node->get_neighbouring_face(Direction::West);
        const Face *face_e = m_node->get_neighbouring_face(Direction::East);

        coefficients.source += m_node->get_dt() * m_node->get_dy() * (face_w->get_field_value(Field::Pressure) - face_e->get_field_value(Field::Pressure));
    } else {
        const Face *face_s = m_node->get_neighbouring_face(Direction::South);
        const Face *face_n = m_node->get_neighbouring_face(Direction::North);

        coefficients.source += m_node->get_dt() * m_node->get_dx() * (face_s->get_field_value(Field::Pressure) - face_n->get_field_value(Field::Pressure));
    }

    return coefficients;
}
