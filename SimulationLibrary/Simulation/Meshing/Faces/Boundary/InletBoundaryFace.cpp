#include "InletBoundaryFace.hpp"

#include <iostream>

#include "../../Nodes/Node.hpp"

InletBoundaryFace::InletBoundaryFace(const double velocity_x, const double velocity_y, const double dye,
                                     const Orientation orientation)
    : BoundaryFace(BoundaryType::Inlet, orientation) {
    m_velocity_x = velocity_x;
    m_velocity_y = velocity_y;
    m_dye = dye;
}

double InletBoundaryFace::get_field_value(const Field field) const {
    switch (field) {
        case Field::VelocityX: {
            return m_velocity_x;
        }
        case Field::VelocityY: {
            return m_velocity_y;
        }
        case Field::Pressure: {
            return m_node_neighbour->get_field_value(Field::Pressure);
        }
        case Field::PressureCorrection: {
            return m_node_neighbour->get_field_value(Field::PressureCorrection);
        }
        case Field::Dye: {
            return m_dye;
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
