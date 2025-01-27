#include "BoundaryFace.hpp"

BoundaryFace::BoundaryFace(const BoundaryType boundary_type, const Orientation orientation)
    : Face(FaceType::Boundary, orientation) {
    m_boundary_type = boundary_type;
}

BoundaryType BoundaryFace::get_boundary_type() const {
    return m_boundary_type;
}

void BoundaryFace::set_node_neighbour(Node *node) {
    m_node_neighbour = node;
}

Node *BoundaryFace::get_node_neighbour() const {
    return m_node_neighbour;
}

double BoundaryFace::get_normal_velocity() {
    if (m_orientation == Orientation::Horizontal) {
        return get_field_value(Field::VelocityX);
    } else {
        return get_field_value(Field::VelocityY);
    }
}
