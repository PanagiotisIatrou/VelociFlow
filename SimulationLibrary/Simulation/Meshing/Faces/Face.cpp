#include "Face.hpp"

#include "Boundary/BoundaryFace.hpp"
#include "Interior/InteriorFace.hpp"

Face::Face(const FaceType type, const Orientation orientation) {
    m_face_type = type;
    m_orientation = orientation;
}

FaceType Face::get_face_type() const {
    return m_face_type;
}

Orientation Face::get_orientation() const {
    return m_orientation;
}

void Face::set_dt(const double dt) {
    m_dt = dt;
}
