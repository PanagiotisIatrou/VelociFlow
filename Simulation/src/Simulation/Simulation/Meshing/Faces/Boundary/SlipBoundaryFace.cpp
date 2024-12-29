#include "SlipBoundaryFace.hpp"

SlipBoundaryFace::SlipBoundaryFace(const Orientation orientation) : BoundaryFace(BoundaryType::Slip, orientation) {}

double SlipBoundaryFace::get_velocity_x() const {
    if (m_orientation == Orientation::Horizontal) {
        return 0.0;
    } else {
        return m_node_neighbour->get_field_value(Field::VelocityX);
    }
}

double SlipBoundaryFace::get_velocity_y() const {
    if (m_orientation == Orientation::Vertical) {
        return 0.0;
    } else {
        return m_node_neighbour->get_field_value(Field::VelocityY);
    }
}

double SlipBoundaryFace::get_dye() const {
    return m_node_neighbour->get_field_value(Field::Dye);
}

double SlipBoundaryFace::get_pressure_correction() const {
    return m_node_neighbour->get_field_value(Field::PressureCorrection);
}

double SlipBoundaryFace::get_pressure() const {
    return m_node_neighbour->get_field_value(Field::Pressure);
}
