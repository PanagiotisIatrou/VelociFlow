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

double Face::get_normal_velocity() {
    if (m_face_type == FaceType::Boundary) {
        if (m_orientation == Orientation::Horizontal) {
            return static_cast<BoundaryFace *>(this)->get_field_value(Field::VelocityX);
        } else {
            return static_cast<BoundaryFace *>(this)->get_field_value(Field::VelocityY);
        }
    } else {
        return static_cast<InteriorFace *>(this)->get_velocity();
    }
}

void Face::set_dt(const double dt) {
    m_dt = dt;
}

double Face::get_viscosity() const {
    return m_viscosity;
}

void Face::set_viscosity(const double viscosity) {
    m_viscosity = viscosity;
}
