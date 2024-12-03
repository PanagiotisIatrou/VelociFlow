#include "MomentumCoefficients.hpp"

#include "../../Node.hpp"
#include "../../../Faces/Boundary/BoundaryFace.hpp"

class Face;

Coefficients MomentumCoefficients::get_central_differencing_diffusion_effects(const Direction direction, const VelocityComponent velocity_component) const {
    Coefficients coefficients;

    Face *face = m_node->get_neighbouring_face(direction);
    double flux = face->get_viscosity() * m_node->m_dt;
    if (direction == Direction::West || direction == Direction::East) {
        flux *= m_node->m_dy / m_node->m_dx;
    } else {
        flux *= m_node->m_dx / m_node->m_dy;
    }
    if (face->get_face_type() != FaceType::Boundary) {
        coefficients.add_to_coefficient(direction, flux);
        coefficients.center += flux;
    } else {
        double face_velocity;
        const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
        if (velocity_component == VelocityComponent::U) {
            face_velocity = boundary_face->get_velocity_u();
        } else {
            face_velocity = boundary_face->get_velocity_v();
        }
        const double extra = 2.0 * flux;
        coefficients.center += extra;
        coefficients.source += face_velocity * extra;
    }

    return coefficients;
}
