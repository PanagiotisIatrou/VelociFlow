#include "MovingWallBoundaryFace.hpp"

#include <iostream>

MovingWallBoundaryFace::MovingWallBoundaryFace(const double velocity, const Orientation orientation)
    : BoundaryFace(BoundaryType::MovingWall, orientation) {
    m_velocity = velocity;
}

double MovingWallBoundaryFace::get_field_value(const Field field) const {
    switch (field) {
        case Field::VelocityX: {
            if (m_orientation == Orientation::Horizontal) {
                return 0.0;
            } else {
                return m_velocity;
            }
        }
        case Field::VelocityY: {
            if (m_orientation == Orientation::Vertical) {
                return 0.0;
            } else {
                return m_velocity;
            }
        }
        case Field::Pressure: {
            return m_node_neighbour->get_field_value(Field::Pressure);
        }
        case Field::PressureCorrection: {
            return m_node_neighbour->get_field_value(Field::PressureCorrection);
        }
        case Field::Dye: {
            return m_node_neighbour->get_field_value(Field::Dye);
        }
        case Field::Phi: {
            return m_node_neighbour->get_field_value(Field::Phi);
        }
        default: {
            std::cerr << std::endl << "Field not recognised" << std::endl;
            exit(1);
        }
    }
}
