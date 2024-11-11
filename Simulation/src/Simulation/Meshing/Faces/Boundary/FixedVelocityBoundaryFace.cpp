#include "FixedVelocityBoundaryFace.hpp"

#include "../../Nodes/Node.hpp"

FixedVelocityBoundaryFace::FixedVelocityBoundaryFace(const double viscosity, const double velocity_u, const double velocity_v) : BoundaryFace(viscosity) {
    m_velocity_u = velocity_u;
    m_velocity_v = velocity_v;
}

double FixedVelocityBoundaryFace::get_velocity_u() const {
    return m_velocity_u;
}

double FixedVelocityBoundaryFace::get_velocity_v() const {
    return m_velocity_v;
}

double FixedVelocityBoundaryFace::get_pressure_correction() const {
    return m_node_neighbour->get_pressure_correction();
}

double FixedVelocityBoundaryFace::get_pressure() const {
    return m_node_neighbour->get_pressure();
}
