#include "FixedValueBoundaryFace.hpp"

#include <iostream>

#include "../../Nodes/Node.hpp"

FixedValueBoundaryFace::FixedValueBoundaryFace(const double phi, const Orientation orientation)
    : BoundaryFace(BoundaryType::FixedValue, orientation) {
    m_phi = phi;
}

double FixedValueBoundaryFace::get_field_value(const Field field) const {
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
        case Field::Phi: {
            return m_phi;
        }
        default: {
            std::cerr << std::endl << "Field not recognised" << std::endl;
            exit(1);
        }
    }
}
