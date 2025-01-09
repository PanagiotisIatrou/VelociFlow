#include "FixedPressureBoundaryFace.hpp"

#include <iostream>

#include "../../Nodes/Node.hpp"

FixedPressureBoundaryFace::FixedPressureBoundaryFace(const double pressure, const Orientation orientation)
    : BoundaryFace(BoundaryType::FixedPressure, orientation) {
    m_pressure = pressure;
}

double FixedPressureBoundaryFace::get_field_value(const Field field) const {
    switch (field) {
        case Field::VelocityX: {
            return m_node_neighbour->get_field_value(Field::VelocityX);
        }
        case Field::VelocityY: {
            return m_node_neighbour->get_field_value(Field::VelocityY);
        }
        case Field::Pressure: {
            return m_pressure;
        }
        case Field::PressureCorrection: {
            return 0.0;
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
