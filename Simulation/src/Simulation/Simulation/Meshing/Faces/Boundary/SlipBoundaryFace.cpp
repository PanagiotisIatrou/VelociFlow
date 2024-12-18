#include "SlipBoundaryFace.hpp"

SlipBoundaryFace::SlipBoundaryFace(const Orientation orientation) : BoundaryFace(BoundaryType::Slip, orientation) {}

double SlipBoundaryFace::get_velocity_u() const {
    if (m_orientation == Orientation::Horizontal) {
        return 0.0;
    } else {
        return m_node_neighbour->get_velocity_u();
    }
}

double SlipBoundaryFace::get_velocity_v() const {
    if (m_orientation == Orientation::Vertical) {
        return 0.0;
    } else {
        return m_node_neighbour->get_velocity_v();
    }
}

double SlipBoundaryFace::get_dye() const {
    return m_node_neighbour->get_dye();
}

double SlipBoundaryFace::get_pressure_correction() const {
    return m_node_neighbour->get_pressure_correction();
}

double SlipBoundaryFace::get_pressure() const {
    return m_node_neighbour->get_pressure();
}
