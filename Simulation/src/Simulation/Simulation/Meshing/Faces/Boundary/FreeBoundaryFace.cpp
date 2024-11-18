#include "FreeBoundaryFace.hpp"

#include "../../Nodes/Node.hpp"

FreeBoundaryFace::FreeBoundaryFace(const double viscosity) : BoundaryFace(BoundaryType::Free, viscosity) {
}

double FreeBoundaryFace::get_velocity_u() const {
    return m_node_neighbour->get_velocity_u();
}

double FreeBoundaryFace::get_velocity_v() const {
    return m_node_neighbour->get_velocity_v();
}

double FreeBoundaryFace::get_pressure_correction() const {
    return m_node_neighbour->get_pressure_correction();
}

double FreeBoundaryFace::get_pressure() const {
    return m_node_neighbour->get_pressure();
}
