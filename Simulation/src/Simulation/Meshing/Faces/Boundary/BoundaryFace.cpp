#include "BoundaryFace.hpp"

BoundaryFace::BoundaryFace(const BoundaryType boundary_type, const double viscosity) : Face(FaceType::Boundary) {
    m_boundary_type = boundary_type;
    m_viscosity = viscosity;
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
