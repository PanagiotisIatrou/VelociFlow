#include "Pressure.hpp"

#include "../../../../Meshing/Faces/Face.hpp"
#include "../../../../Meshing/Nodes/Node.hpp"

Pressure::Pressure(Node *node, const Component component) : TermDiscretization(node) {
    m_node = node;
    m_component = component;
}

Coefficients Pressure::get_effects() const {
    Coefficients coefficients;

    if (m_component == Component::X) {
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
