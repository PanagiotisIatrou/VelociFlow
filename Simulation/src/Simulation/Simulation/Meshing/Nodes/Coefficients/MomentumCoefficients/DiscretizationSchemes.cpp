#include "DiscretizationSchemes.hpp"

#include <cmath>

#include "../../Node.hpp"
#include "../../../Faces/Boundary/BoundaryFace.hpp"

DiscretizationSchemes::DiscretizationSchemes(Node *node) {
    m_node = node;
}

SchemeCoefficients DiscretizationSchemes::convection(const Direction direction, const Scheme scheme, const VelocityComponent velocity_component) {
    Face *face = m_node->get_neighbouring_face(direction);
    const double velocity_u_f = face->get_normal_velocity();
    const double area = direction == Direction::West || direction == Direction::East ? m_node->m_dy : m_node->m_dx;
    const double factor = m_node->m_density * area * m_node->m_dt * velocity_u_f;

    if (face->get_face_type() != FaceType::Boundary) {
        if (scheme == Scheme::Upwind) {
            return upwind(velocity_u_f, factor);
        } else if (scheme == Scheme::CentralDifferencing) {
            return central_differencing(factor);
        } else if (scheme == Scheme::Hybrid) {
            return hybrid(area, velocity_u_f, m_node->m_viscosity, m_node->m_density, factor);
        }
    } else {
        const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
        double velocity_f;
        if (velocity_component == VelocityComponent::U) {
            velocity_f = boundary_face->get_velocity_u();
        } else {
            velocity_f = boundary_face->get_velocity_v();
        }
        return {0.0, 0.0, factor * velocity_f};
    }
}

SchemeCoefficients DiscretizationSchemes::upwind(const double velocity, const double factor) {
    if (velocity > 0.0) {
        return {factor, 0.0, 0.0};
    } else if (velocity < 0.0) {
        return {0.0, factor, 0.0};
    } else {
        return {0.0, 0.0, 0.0};
    }
}

SchemeCoefficients DiscretizationSchemes::central_differencing(const double factor) {
    return {0.5 * factor, 0.5 * factor, 0.0};
}

SchemeCoefficients DiscretizationSchemes::hybrid(const double length, const double velocity, const double viscosity, const double density, const double factor) {
    const double peclet = length * velocity * density / viscosity;
    if (std::abs(peclet) < 2.0) {
        return central_differencing(factor);
    } else {
        return upwind(velocity, factor);
    }
}
