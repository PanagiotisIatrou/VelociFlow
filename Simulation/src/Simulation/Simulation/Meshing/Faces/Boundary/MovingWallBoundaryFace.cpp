#include "MovingWallBoundaryFace.hpp"

MovingWallBoundaryFace::MovingWallBoundaryFace(const double velocity, const Orientation orientation) : BoundaryFace(BoundaryType::MovingWall, orientation) {
    m_velocity = velocity;
}

double MovingWallBoundaryFace::get_velocity_u() const {
    if (m_orientation == Orientation::Horizontal) {
        return 0.0;
    } else {
        return m_velocity;
    }
}

double MovingWallBoundaryFace::get_velocity_v() const {
    if (m_orientation == Orientation::Vertical) {
        return 0.0;
    } else {
        return m_velocity;
    }
}

double MovingWallBoundaryFace::get_dye() const {
    return m_node_neighbour->get_dye();
}

double MovingWallBoundaryFace::get_pressure_correction() const {
    return m_node_neighbour->get_pressure_correction();
}

double MovingWallBoundaryFace::get_pressure() const {
    return m_node_neighbour->get_pressure();
}
