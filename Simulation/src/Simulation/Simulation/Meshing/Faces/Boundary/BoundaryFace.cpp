#include "BoundaryFace.hpp"

BoundaryFace::BoundaryFace(const BoundaryType boundary_type) : Face(FaceType::Boundary) {
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

double BoundaryFace::get_viscosity() const {
    return m_node_neighbour->get_viscosity();
}

double BoundaryFace::get_density() const {
    return m_node_neighbour->get_density();
}
