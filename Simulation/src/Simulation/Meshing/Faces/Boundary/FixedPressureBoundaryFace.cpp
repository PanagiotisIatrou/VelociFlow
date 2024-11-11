#include "FixedPressureBoundaryFace.hpp"

#include "../../Nodes/Node.hpp"

FixedPressureBoundaryFace::FixedPressureBoundaryFace(double viscosity, double pressure) : BoundaryFace(viscosity) {
    m_pressure = pressure;
}

double FixedPressureBoundaryFace::get_velocity_u() const {
    return m_node_neighbour->get_velocity_u();
}

double FixedPressureBoundaryFace::get_velocity_v() const {
    return m_node_neighbour->get_velocity_v();
}

double FixedPressureBoundaryFace::get_pressure_correction() const {
    return 0.0;
}

double FixedPressureBoundaryFace::get_pressure() const {
    return m_pressure;
}
