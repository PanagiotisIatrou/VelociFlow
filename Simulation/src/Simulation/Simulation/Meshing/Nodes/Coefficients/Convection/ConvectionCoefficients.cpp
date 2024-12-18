#include "ConvectionCoefficients.hpp"

#include "../../../Faces/Boundary/BoundaryFace.hpp"

class Face;

ConvectionCoefficients::ConvectionCoefficients(Node *node, const bool include_density) {
    m_node = node;
    m_include_density = include_density;
}

Coefficients ConvectionCoefficients::get_convection_effects(const VelocityComponent velocity_component,
                                                            const ConvectionSchemes convection_scheme) const {
    Coefficients coefficients;

    for (int dir = 0; dir < direction_near_end; dir++) {
        const Direction direction = static_cast<Direction>(dir);
        Coefficients direction_coefficients;
        switch (convection_scheme) {
            case ConvectionSchemes::Upwind: {
                direction_coefficients = get_upwind_convection_effects(direction, velocity_component);
                break;
            }
            case ConvectionSchemes::CentralDifferencing: {
                direction_coefficients = get_central_differencing_convection_effects(direction, velocity_component);
                break;
            }
            case ConvectionSchemes::QuickHayase: {
                direction_coefficients = get_quick_hayase_convection_effects(direction, velocity_component);
                break;
            }
        }
        coefficients += direction_coefficients;
    }

    return coefficients;
}

Coefficients ConvectionCoefficients::get_upwind_convection_effects(const Direction direction,
                                                                   const VelocityComponent velocity_component) const {
    Coefficients coefficients;

    Face *face = m_node->get_neighbouring_face(direction);
    const double area = direction == Direction::West || direction == Direction::East
                            ? m_node->get_dy()
                            : m_node->get_dx();
    double flow_rate = m_node->get_dt() * area * face->get_normal_velocity();
    if (m_include_density) {
        flow_rate *= face->get_density();
    }

    const int dir_sign = direction == Direction::West || direction == Direction::South ? 1 : -1;
    if (face->get_face_type() != FaceType::Boundary) {
        const double extra = std::max(dir_sign * flow_rate, 0.0);
        coefficients.add_to_coefficient(direction, extra);
        coefficients.center += extra - dir_sign * flow_rate;
    } else {
        double face_velocity;
        const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
        if (velocity_component == VelocityComponent::U) {
            face_velocity = boundary_face->get_velocity_u();
        } else {
            face_velocity = boundary_face->get_velocity_v();
        }
        coefficients.source += dir_sign * flow_rate * face_velocity;
    }

    return coefficients;
}

Coefficients ConvectionCoefficients::get_central_differencing_convection_effects(
    const Direction direction, const VelocityComponent velocity_component) const {
    Coefficients coefficients;

    Face *face = m_node->get_neighbouring_face(direction);
    const double area = direction == Direction::West || direction == Direction::East
                            ? m_node->get_dy()
                            : m_node->get_dx();
    double flow_rate = m_node->get_dt() * area * face->get_normal_velocity();
    if (m_include_density) {
        flow_rate *= face->get_density();
    }
    const int dir_sign = direction == Direction::West || direction == Direction::South ? 1 : -1;
    if (face->get_face_type() != FaceType::Boundary) {
        const double extra = 0.5 * dir_sign * flow_rate;
        coefficients.add_to_coefficient(direction, extra);
        coefficients.center += extra - dir_sign * flow_rate;
    } else {
        double face_velocity;
        const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
        if (velocity_component == VelocityComponent::U) {
            face_velocity = boundary_face->get_velocity_u();
        } else {
            face_velocity = boundary_face->get_velocity_v();
        }
        coefficients.source += -dir_sign * flow_rate * face_velocity;
    }

    return coefficients;
}

Coefficients ConvectionCoefficients::get_quick_hayase_convection_effects(
    const Direction direction, const VelocityComponent velocity_component) const {
    Coefficients coefficients;

    Face *face = m_node->get_neighbouring_face(direction);
    const double area = direction == Direction::West || direction == Direction::East
                            ? m_node->get_dy()
                            : m_node->get_dx();
    double flow_rate = m_node->get_dt() * area * face->get_normal_velocity();
    if (m_include_density) {
        flow_rate *= face->get_density();
    }
    const double dir_sign = direction == Direction::West || direction == Direction::South ? 1.0 : -1.0;
    if (face->get_face_type() != FaceType::Boundary) {
        const double alpha = flow_rate > 0.0 ? 1.0 : 0.0;
        const double extra = alpha * flow_rate - (direction == Direction::East || direction == Direction::North
                                                      ? flow_rate
                                                      : 0);
        coefficients.add_to_coefficient(direction, extra);
        coefficients.center += extra - dir_sign * flow_rate;

        // Get node value
        double node_value;
        if (velocity_component == VelocityComponent::U) {
            node_value = m_node->get_velocity_u();
        } else {
            node_value = m_node->get_velocity_v();
        }

        // Get opposite direction node value
        const Direction opposite_direction = get_opposite_direction(direction);
        Face *opposite_direction_face = m_node->get_neighbouring_face(opposite_direction);
        double opposite_node_value;
        if (opposite_direction_face->get_face_type() == FaceType::Interior) {
            const Node *opposite_node = m_node->get_neighbouring_node(opposite_direction);
            if (velocity_component == VelocityComponent::U) {
                opposite_node_value = opposite_node->get_velocity_u();
            } else {
                opposite_node_value = opposite_node->get_velocity_v();
            }
        } else {
            double boundary_value;
            const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(opposite_direction_face);
            if (velocity_component == VelocityComponent::U) {
                boundary_value = boundary_face->get_velocity_u();
            } else {
                boundary_value = boundary_face->get_velocity_v();
            }
            opposite_node_value = 2 * boundary_value - node_value;
        }

        // Get direction node value
        double direction_node_value;
        const Node *direction_node = m_node->get_neighbouring_node(direction);
        if (velocity_component == VelocityComponent::U) {
            direction_node_value = direction_node->get_velocity_u();
        } else {
            direction_node_value = direction_node->get_velocity_v();
        }

        // Get extended node value
        const Direction extended_direction = extend_direction(direction);
        Face *extended_direction_face = m_node->get_neighbouring_face(extended_direction);
        double extended_node_value;
        if (extended_direction_face->get_face_type() == FaceType::Interior) {
            const Node *extended_node = m_node->get_neighbouring_node(extended_direction);
            if (velocity_component == VelocityComponent::U) {
                extended_node_value = extended_node->get_velocity_u();
            } else {
                extended_node_value = extended_node->get_velocity_v();
            }
        } else {
            // Mirror node
            double boundary_value;
            const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(extended_direction_face);
            if (velocity_component == VelocityComponent::U) {
                boundary_value = boundary_face->get_velocity_u();
            } else {
                boundary_value = boundary_face->get_velocity_v();
            }

            extended_node_value = 2 * boundary_value - direction_node_value;
        }

        // Source
        const double fac1 = direction == Direction::West || direction == Direction::South ? alpha : 1 - alpha;
        const double fac2 = direction == Direction::West || direction == Direction::South ? 1 - alpha : alpha;
        coefficients.source += dir_sign * (1.0 / 8.0) * fac1 * flow_rate * (
            3 * node_value - 2 * direction_node_value - extended_node_value);
        coefficients.source += dir_sign * (1.0 / 8.0) * fac2 * flow_rate * (
            3 * direction_node_value - 2 * node_value - opposite_node_value);
    } else {
        double face_velocity;
        const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
        if (velocity_component == VelocityComponent::U) {
            face_velocity = boundary_face->get_velocity_u();
        } else {
            face_velocity = boundary_face->get_velocity_v();
        }
        coefficients.source += dir_sign * flow_rate * face_velocity;
    }

    return coefficients;
}
