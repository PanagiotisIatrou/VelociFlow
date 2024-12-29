#include "FixedPressureBoundaryFace.hpp"

#include "../../Nodes/Node.hpp"

FixedPressureBoundaryFace::FixedPressureBoundaryFace(const double pressure, const Orientation orientation) : BoundaryFace(BoundaryType::FixedPressure, orientation) {
    m_pressure = pressure;
}

double FixedPressureBoundaryFace::get_velocity_x() const {
    return m_node_neighbour->get_field_value(Field::VelocityX);
}

double FixedPressureBoundaryFace::get_velocity_y() const {
    return m_node_neighbour->get_field_value(Field::VelocityY);
}

double FixedPressureBoundaryFace::get_dye() const {
    return m_node_neighbour->get_field_value(Field::Dye);
}

double FixedPressureBoundaryFace::get_pressure_correction() const {
    return 0.0;
}

double FixedPressureBoundaryFace::get_pressure() const {
    return m_pressure;
}
