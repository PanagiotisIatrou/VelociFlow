#include "Convection.hpp"

#include "../../../../Meshing/Faces/Boundary/BoundaryFace.hpp"

class Face;

Convection::Convection(Node *node, const Field field, const ConvectionSchemes convection_scheme,
                       const bool include_density)
    : TermDiscretization(node) {
    m_include_density = include_density;
    m_convection_scheme = convection_scheme;
    m_field = field;
}

Coefficients Convection::get_effects() const {
    Coefficients coefficients;

    for (int dir = 0; dir < direction_near_end; dir++) {
        const Direction direction = static_cast<Direction>(dir);
        Coefficients direction_coefficients;
        switch (m_convection_scheme) {
            case ConvectionSchemes::Upwind: {
                direction_coefficients = get_upwind_effects(direction);
                break;
            }
            case ConvectionSchemes::CentralDifferencing: {
                direction_coefficients = get_central_differencing_effects(direction);
                break;
            }
            case ConvectionSchemes::QuickHayase: {
                direction_coefficients = get_quick_hayase_effects(direction);
                break;
            }
        }
        coefficients += direction_coefficients;
    }

    // Multiply by density
    if (m_include_density) {
        coefficients *= m_node->get_density();
    }

    return coefficients;
}

Coefficients Convection::get_upwind_effects(const Direction direction) const {
    Coefficients coefficients;

    Face *face = m_node->get_neighbouring_face(direction);
    const double area =
        direction == Direction::West || direction == Direction::East ? m_node->get_dy() : m_node->get_dx();
    const double flow_rate = m_node->get_dt() * area * face->get_normal_velocity();

    const int dir_sign = direction == Direction::West || direction == Direction::South ? 1 : -1;
    if (face->get_face_type() != FaceType::Boundary) {
        const double extra = std::max(dir_sign * flow_rate, 0.0);
        coefficients.add_to_coefficient(direction, extra);
        coefficients.center += extra - dir_sign * flow_rate;
    } else {
        const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
        const double face_value = boundary_face->get_field_value(m_field);
        coefficients.source += dir_sign * flow_rate * face_value;
    }

    return coefficients;
}

Coefficients Convection::get_central_differencing_effects(const Direction direction) const {
    Coefficients coefficients;

    Face *face = m_node->get_neighbouring_face(direction);
    const double area =
        direction == Direction::West || direction == Direction::East ? m_node->get_dy() : m_node->get_dx();
    const double flow_rate = m_node->get_dt() * area * face->get_normal_velocity();
    const int dir_sign = direction == Direction::West || direction == Direction::South ? 1 : -1;
    if (face->get_face_type() != FaceType::Boundary) {
        const double extra = 0.5 * dir_sign * flow_rate;
        coefficients.add_to_coefficient(direction, extra);
        coefficients.center += extra - dir_sign * flow_rate;
    } else {
        const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
        const double face_value = boundary_face->get_field_value(m_field);
        coefficients.source += -dir_sign * flow_rate * face_value;
    }

    return coefficients;
}

Coefficients Convection::get_quick_hayase_effects(const Direction direction) const {
    Coefficients coefficients;

    Face *face = m_node->get_neighbouring_face(direction);
    const double area =
        direction == Direction::West || direction == Direction::East ? m_node->get_dy() : m_node->get_dx();
    const double flow_rate = m_node->get_dt() * area * face->get_normal_velocity();
    const double dir_sign = direction == Direction::West || direction == Direction::South ? 1.0 : -1.0;
    if (face->get_face_type() != FaceType::Boundary) {
        const double alpha = flow_rate > 0.0 ? 1.0 : 0.0;
        const double extra =
            alpha * flow_rate - (direction == Direction::East || direction == Direction::North ? flow_rate : 0);
        coefficients.add_to_coefficient(direction, extra);
        coefficients.center += extra - dir_sign * flow_rate;

        // Get node value
        const double node_value = m_node->get_field_value(m_field);

        // Get opposite direction node value
        const Direction opposite_direction = get_opposite_direction(direction);
        Face *opposite_direction_face = m_node->get_neighbouring_face(opposite_direction);
        double opposite_node_value;
        if (opposite_direction_face->get_face_type() == FaceType::Interior) {
            const Node *opposite_node = m_node->get_neighbouring_node(opposite_direction);
            opposite_node_value = opposite_node->get_field_value(m_field);
        } else {
            const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(opposite_direction_face);
            const double boundary_value = boundary_face->get_field_value(m_field);
            opposite_node_value = 2 * boundary_value - node_value;
        }

        // Get direction node value
        const Node *direction_node = m_node->get_neighbouring_node(direction);
        const double direction_node_value = direction_node->get_field_value(m_field);

        // Get extended node value
        const Direction extended_direction = extend_direction(direction);
        Face *extended_direction_face = m_node->get_neighbouring_face(extended_direction);
        double extended_node_value;
        if (extended_direction_face->get_face_type() == FaceType::Interior) {
            const Node *extended_node = m_node->get_neighbouring_node(extended_direction);
            extended_node_value = extended_node->get_field_value(m_field);
        } else {
            // Mirror node
            const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(extended_direction_face);
            const double boundary_value = boundary_face->get_field_value(m_field);
            extended_node_value = 2 * boundary_value - direction_node_value;
        }

        // Source
        const double fac1 = direction == Direction::West || direction == Direction::South ? alpha : 1 - alpha;
        const double fac2 = direction == Direction::West || direction == Direction::South ? 1 - alpha : alpha;
        coefficients.source += dir_sign * (1.0 / 8.0) * fac1 * flow_rate *
                               (3 * node_value - 2 * direction_node_value - extended_node_value);
        coefficients.source += dir_sign * (1.0 / 8.0) * fac2 * flow_rate *
                               (3 * direction_node_value - 2 * node_value - opposite_node_value);
    } else {
        const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
        const double face_velocity = boundary_face->get_field_value(m_field);
        coefficients.source += dir_sign * flow_rate * face_velocity;
    }

    return coefficients;
}
