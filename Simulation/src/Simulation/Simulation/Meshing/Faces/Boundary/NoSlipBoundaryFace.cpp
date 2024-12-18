#include "NoSlipBoundaryFace.hpp"

NoSlipBoundaryFace::NoSlipBoundaryFace(const Orientation orientation) : BoundaryFace(BoundaryType::NoSlip, orientation) {}

double NoSlipBoundaryFace::get_velocity_u() const {
    return 0.0;
}

double NoSlipBoundaryFace::get_velocity_v() const {
    return 0.0;
}

double NoSlipBoundaryFace::get_dye() const {
    return m_node_neighbour->get_dye();
}

double NoSlipBoundaryFace::get_pressure_correction() const {
    return m_node_neighbour->get_pressure_correction();
}

double NoSlipBoundaryFace::get_pressure() const {
    return m_node_neighbour->get_pressure();
}
