#include "NoSlipBoundaryFace.hpp"

NoSlipBoundaryFace::NoSlipBoundaryFace(const Orientation orientation) : BoundaryFace(BoundaryType::NoSlip, orientation) {}

double NoSlipBoundaryFace::get_velocity_u() const {
    return 0.0;
}

double NoSlipBoundaryFace::get_velocity_v() const {
    return 0.0;
}

double NoSlipBoundaryFace::get_dye() const {
    return m_node_neighbour->get_field_value(Field::Dye);
}

double NoSlipBoundaryFace::get_pressure_correction() const {
    return m_node_neighbour->get_field_value(Field::PressureCorrection);
}

double NoSlipBoundaryFace::get_pressure() const {
    return m_node_neighbour->get_field_value(Field::Pressure);
}
