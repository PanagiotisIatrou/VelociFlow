#include "Face.hpp"

Face::Face(const FaceType type) {
    m_face_type = type;
}

FaceType Face::get_face_type() const {
    return m_face_type;
}

double Face::get_viscosity() const {
    return m_viscosity;
}
