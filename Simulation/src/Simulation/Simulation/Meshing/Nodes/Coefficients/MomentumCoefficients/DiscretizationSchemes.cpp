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
    const double area = direction == Direction::WestWest || direction == Direction::West || direction == Direction::East || direction == Direction::EastEast ? m_node->m_dy : m_node->m_dx;
    const double flow_rate_f = m_node->m_density * area * m_node->m_dt * velocity_u_f;

    if (face->get_face_type() != FaceType::Boundary) {
        switch (scheme) {
            case Scheme::Upwind:
                return upwind(flow_rate_f);
            case Scheme::CentralDifferencing:
                return central_differencing(flow_rate_f);
            case Scheme::Hybrid:
                const double length = direction == Direction::West || direction == Direction::East ? m_node->m_dx : m_node->m_dy;
                return hybrid(length, velocity_u_f, m_node->m_viscosity, m_node->m_density, flow_rate_f);
        }
    } else {
        const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
        double velocity_f;
        if (velocity_component == VelocityComponent::U) {
            velocity_f = boundary_face->get_velocity_u();
        } else {
            velocity_f = boundary_face->get_velocity_v();
        }
        return {
            .node0 = 0.0,
            .node1 = 0.0,
            .node2 = 0.0,
            .node3 = 0.0,
            .source = flow_rate_f * velocity_f
        };
    }
}

SchemeCoefficients DiscretizationSchemes::upwind(const double flow_rate) {
    return {
        .node0 = 0.0,
        .node1 = std::max(flow_rate, 0.0),
        .node2 = -std::max(-flow_rate, 0.0),
        .node3 = 0.0,
        .source = 0.0
    };
}

SchemeCoefficients DiscretizationSchemes::central_differencing(const double flow_rate) {
    return {
        .node0 = 0.0,
        .node1 = 0.5 * flow_rate,
        .node2 = 0.5 * flow_rate,
        .node3 = 0.0,
        .source = 0.0
    };
}

SchemeCoefficients DiscretizationSchemes::hybrid(const double length, const double velocity, const double viscosity, const double density, const double flow_rate) {
    const double peclet = length * velocity * density / viscosity;
    if (std::abs(peclet) < 2.0) {
        return central_differencing(flow_rate);
    } else {
        return upwind(flow_rate);
    }
}
