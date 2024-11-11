#include "BoundaryFace.hpp"

BoundaryFace::BoundaryFace(const double viscosity) : Face(FaceType::Boundary) {
    m_viscosity = viscosity;
}

void BoundaryFace::set_node_neighbour(Node *node) {
    m_node_neighbour = node;
}

Node *BoundaryFace::get_node_neighbour() const {
    return m_node_neighbour;
}
