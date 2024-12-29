#include "InletBoundaryFace.hpp"

#include "../../Nodes/Node.hpp"

InletBoundaryFace::InletBoundaryFace(const double velocity_x, const double velocity_y, const double dye, const Orientation orientation) : BoundaryFace(BoundaryType::Inlet, orientation) {
    m_velocity_x = velocity_x;
    m_velocity_y = velocity_y;
    m_dye = dye;
}

double InletBoundaryFace::get_velocity_x() const {
    return m_velocity_x;
}

double InletBoundaryFace::get_velocity_y() const {
    return m_velocity_y;
}

double InletBoundaryFace::get_dye() const {
    return m_dye;
}

double InletBoundaryFace::get_pressure_correction() const {
    return m_node_neighbour->get_field_value(Field::PressureCorrection);
}

double InletBoundaryFace::get_pressure() const {
    return m_node_neighbour->get_field_value(Field::Pressure);
}
