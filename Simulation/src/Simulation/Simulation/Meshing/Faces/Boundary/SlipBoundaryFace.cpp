#include "SlipBoundaryFace.hpp"

#include <iostream>

SlipBoundaryFace::SlipBoundaryFace(const Orientation orientation) : BoundaryFace(BoundaryType::Slip, orientation) {}

double SlipBoundaryFace::get_field_value(const Field field) const {
    switch (field) {
        case Field::VelocityX : {
            if (m_orientation == Orientation::Horizontal) {
                return 0.0;
            } else {
                return m_node_neighbour->get_field_value(Field::VelocityX);
            }
        }
        case Field::VelocityY : {
            if (m_orientation == Orientation::Vertical) {
                return 0.0;
            } else {
                return m_node_neighbour->get_field_value(Field::VelocityY);
            }
        }
        case Field::Pressure : {
            return m_node_neighbour->get_field_value(Field::Pressure);
        }
        case Field::PressureCorrection : {
            return m_node_neighbour->get_field_value(Field::PressureCorrection);
        }
        case Field::Dye : {
            return m_node_neighbour->get_field_value(Field::Dye);
        }
        default : {
            std::cerr << std::endl << "Field not recognised" << std::endl;
            exit(1);
        }
    }
}
