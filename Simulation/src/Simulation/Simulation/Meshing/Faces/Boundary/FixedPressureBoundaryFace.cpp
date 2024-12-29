#include "FixedPressureBoundaryFace.hpp"

#include "../../Nodes/Node.hpp"

FixedPressureBoundaryFace::FixedPressureBoundaryFace(const double pressure, const Orientation orientation) : BoundaryFace(BoundaryType::FixedPressure, orientation) {
    m_pressure = pressure;
}

double FixedPressureBoundaryFace::get_velocity_u() const {
    return m_node_neighbour->get_velocity_x();
}

double FixedPressureBoundaryFace::get_velocity_v() const {
    return m_node_neighbour->get_velocity_y();
}

double FixedPressureBoundaryFace::get_dye() const {
    return m_node_neighbour->get_dye();
}

double FixedPressureBoundaryFace::get_pressure_correction() const {
    return 0.0;
}

double FixedPressureBoundaryFace::get_pressure() const {
    return m_pressure;
}
