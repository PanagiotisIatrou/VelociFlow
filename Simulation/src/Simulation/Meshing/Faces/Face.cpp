#include "Face.hpp"

Face::Face(const FaceType type) {
    m_type = type;
}

FaceType Face::get_type() const {
    return m_type;
}

double Face::get_viscosity() const {
    return m_viscosity;
}
