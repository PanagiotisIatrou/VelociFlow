#include "NoSlipBoundaryFace.hpp"

#include <iostream>

NoSlipBoundaryFace::NoSlipBoundaryFace(const Orientation orientation) : BoundaryFace(BoundaryType::NoSlip, orientation) {}

double NoSlipBoundaryFace::get_field_value(const Field field) const {
    switch (field) {
        case Field::VelocityX : {
            return 0.0;
        }
        case Field::VelocityY : {
            return 0.0;
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
        case Field::Density : {
            return m_node_neighbour->get_field_value(Field::Density);
        }
        case Field::Viscosity : {
            return m_node_neighbour->get_field_value(Field::Viscosity);
        }
        default : {
            std::cerr << std::endl << "Field not recognised" << std::endl;
            exit(1);
        }
    }
}
