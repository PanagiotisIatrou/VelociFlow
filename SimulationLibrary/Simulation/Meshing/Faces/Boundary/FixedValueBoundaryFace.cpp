#include "FixedValueBoundaryFace.hpp"

#include <iostream>

#include "../../Nodes/Node.hpp"

FixedValueBoundaryFace::FixedValueBoundaryFace(const double phi, const Orientation orientation)
    : BoundaryFace(BoundaryType::FixedValue, orientation) {
    m_phi = phi;
}

double FixedValueBoundaryFace::get_field_value(const Field field) const {
    switch (field) {
        case Field::Phi: {
            return m_phi;
        }
        default: {
            std::cerr << std::endl << "Field not recognised" << std::endl;
            exit(1);
        }
    }
}
