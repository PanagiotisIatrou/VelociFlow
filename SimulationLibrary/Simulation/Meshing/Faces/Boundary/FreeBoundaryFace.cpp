#include "FreeBoundaryFace.hpp"

#include <iostream>

#include "../../Nodes/Node.hpp"

FreeBoundaryFace::FreeBoundaryFace(const Orientation orientation) : BoundaryFace(BoundaryType::Free, orientation) {
}

double FreeBoundaryFace::get_field_value(const Field field) const {
    switch (field) {
        case Field::VelocityX: {
            return m_node_neighbour->get_field_value(Field::VelocityX);
        }
        case Field::VelocityY: {
            return m_node_neighbour->get_field_value(Field::VelocityY);
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
        default: {
            std::cerr << std::endl << "Field not recognised" << std::endl;
            exit(1);
        }
    }
}
