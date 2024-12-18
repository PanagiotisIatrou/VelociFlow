#include "InletBoundaryFace.hpp"

#include "../../Nodes/Node.hpp"

InletBoundaryFace::InletBoundaryFace(const double velocity_u, const double velocity_v, const double dye, const Orientation orientation) : BoundaryFace(BoundaryType::Inlet, orientation) {
    m_velocity_u = velocity_u;
    m_velocity_v = velocity_v;
    m_dye = dye;
}

double InletBoundaryFace::get_velocity_u() const {
    return m_velocity_u;
}

double InletBoundaryFace::get_velocity_v() const {
    return m_velocity_v;
}

double InletBoundaryFace::get_dye() const {
    return m_dye;
}

double InletBoundaryFace::get_pressure_correction() const {
    return m_node_neighbour->get_pressure_correction();
}

double InletBoundaryFace::get_pressure() const {
    return m_node_neighbour->get_pressure();
}
