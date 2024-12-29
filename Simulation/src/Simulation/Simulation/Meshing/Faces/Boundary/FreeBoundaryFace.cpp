#include "FreeBoundaryFace.hpp"

#include "../../Nodes/Node.hpp"

FreeBoundaryFace::FreeBoundaryFace(const Orientation orientation) : BoundaryFace(BoundaryType::Free, orientation) {
}

double FreeBoundaryFace::get_velocity_u() const {
    return m_node_neighbour->get_field_value(Field::VelocityX);
}

double FreeBoundaryFace::get_velocity_v() const {
    return m_node_neighbour->get_field_value(Field::VelocityY);
}

double FreeBoundaryFace::get_dye() const {
    return m_node_neighbour->get_field_value(Field::Dye);
}

double FreeBoundaryFace::get_pressure_correction() const {
    return m_node_neighbour->get_field_value(Field::PressureCorrection);
}

double FreeBoundaryFace::get_pressure() const {
    return m_node_neighbour->get_field_value(Field::Pressure);
}
