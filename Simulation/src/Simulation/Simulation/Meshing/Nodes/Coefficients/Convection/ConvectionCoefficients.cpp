#include "ConvectionCoefficients.hpp"

#include <iostream>

#include "../../../Faces/Boundary/BoundaryFace.hpp"

class Face;

ConvectionCoefficients::ConvectionCoefficients(Node *node, const bool include_density) {
    m_node = node;
    m_include_density = include_density;
}

Coefficients ConvectionCoefficients::get_convection_effects(const Field field,
                                                            const ConvectionSchemes convection_scheme) const {
    Coefficients coefficients;

    for (int dir = 0; dir < direction_near_end; dir++) {
        const Direction direction = static_cast<Direction>(dir);
        Coefficients direction_coefficients;
        switch (convection_scheme) {
            case ConvectionSchemes::Upwind: {
                direction_coefficients = get_upwind_convection_effects(direction, field);
                break;
            }
            case ConvectionSchemes::CentralDifferencing: {
                direction_coefficients = get_central_differencing_convection_effects(direction, field);
                break;
            }
            case ConvectionSchemes::QuickHayase: {
                direction_coefficients = get_quick_hayase_convection_effects(direction, field);
                break;
            }
        }
        coefficients += direction_coefficients;
    }

    return coefficients;
}

Coefficients ConvectionCoefficients::get_upwind_convection_effects(const Direction direction, const Field field) const {
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
        double face_value;
        const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
        if (field == Field::VelocityX) {
            face_value = boundary_face->get_velocity_u();
        } else if (field == Field::VelocityY) {
            face_value = boundary_face->get_velocity_v();
        } else if (field == Field::Dye) {
            face_value = boundary_face->get_dye();
        } else {
            std::cerr << std::endl << "Field not recognised" << std::endl;
            exit(1);
        }
        coefficients.source += dir_sign * flow_rate * face_value;
    }

    return coefficients;
}

Coefficients ConvectionCoefficients::get_central_differencing_convection_effects(
    const Direction direction, const Field field) const {
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
        double face_value;
        const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
        if (field == Field::VelocityX) {
            face_value = boundary_face->get_velocity_u();
        } else if (field == Field::VelocityY) {
            face_value = boundary_face->get_velocity_v();
        } else if (field == Field::Dye) {
            face_value = boundary_face->get_dye();
        } else {
            std::cerr << std::endl << "Field not recognised" << std::endl;
            exit(1);
        }
        coefficients.source += -dir_sign * flow_rate * face_value;
    }

    return coefficients;
}

Coefficients ConvectionCoefficients::get_quick_hayase_convection_effects(
    const Direction direction, const Field field) const {
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
        if (field == Field::VelocityX) {
            node_value = m_node->get_velocity_u();
        } else if (field == Field::VelocityY) {
            node_value = m_node->get_velocity_v();
        } else if (field == Field::Dye) {
            node_value = m_node->get_dye();
        } else {
            std::cerr << std::endl << "Field not recognised" << std::endl;
            exit(1);
        }

        // Get opposite direction node value
        const Direction opposite_direction = get_opposite_direction(direction);
        Face *opposite_direction_face = m_node->get_neighbouring_face(opposite_direction);
        double opposite_node_value;
        if (opposite_direction_face->get_face_type() == FaceType::Interior) {
            const Node *opposite_node = m_node->get_neighbouring_node(opposite_direction);
            if (field == Field::VelocityX) {
                opposite_node_value = opposite_node->get_velocity_u();
            } else if (field == Field::VelocityY) {
                opposite_node_value = opposite_node->get_velocity_v();
            } else if (field == Field::Dye) {
                opposite_node_value = opposite_node->get_dye();
            } else {
                std::cerr << std::endl << "Field not recognised" << std::endl;
                exit(1);
            }
        } else {
            double boundary_value;
            const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(opposite_direction_face);
            if (field == Field::VelocityX) {
                boundary_value = boundary_face->get_velocity_u();
            } else if (field == Field::VelocityY) {
                boundary_value = boundary_face->get_velocity_v();
            } else if (field == Field::Dye) {
                boundary_value = boundary_face->get_dye();
            } else {
                std::cerr << std::endl << "Field not recognised" << std::endl;
                exit(1);
            }
            opposite_node_value = 2 * boundary_value - node_value;
        }

        // Get direction node value
        double direction_node_value;
        const Node *direction_node = m_node->get_neighbouring_node(direction);
        if (field == Field::VelocityX) {
            direction_node_value = direction_node->get_velocity_u();
        } else if (field == Field::VelocityY) {
            direction_node_value = direction_node->get_velocity_v();
        } else if (field == Field::Dye) {
            direction_node_value = direction_node->get_dye();
        } else {
            std::cerr << std::endl << "Field not recognised" << std::endl;
            exit(1);
        }

        // Get extended node value
        const Direction extended_direction = extend_direction(direction);
        Face *extended_direction_face = m_node->get_neighbouring_face(extended_direction);
        double extended_node_value;
        if (extended_direction_face->get_face_type() == FaceType::Interior) {
            const Node *extended_node = m_node->get_neighbouring_node(extended_direction);
            if (field == Field::VelocityX) {
                extended_node_value = extended_node->get_velocity_u();
            } else if (field == Field::VelocityY) {
                extended_node_value = extended_node->get_velocity_v();
            } else if (field == Field::Dye) {
                extended_node_value = extended_node->get_dye();
            } else {
                std::cerr << std::endl << "Field not recognised" << std::endl;
                exit(1);
            }
        } else {
            // Mirror node
            double boundary_value;
            const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(extended_direction_face);
            if (field == Field::VelocityX) {
                boundary_value = boundary_face->get_velocity_u();
            } else if (field == Field::VelocityY) {
                boundary_value = boundary_face->get_velocity_v();
            } else if (field == Field::Dye) {
                boundary_value = boundary_face->get_dye();
            } else {
                std::cerr << std::endl << "Field not recognised" << std::endl;
                exit(1);
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
        if (field == Field::VelocityX) {
            face_velocity = boundary_face->get_velocity_u();
        } else if (field == Field::VelocityY) {
            face_velocity = boundary_face->get_velocity_v();
        } else if (field == Field::Dye) {
            face_velocity = boundary_face->get_dye();
        } else {
            std::cerr << std::endl << "Field not recognised" << std::endl;
            exit(1);
        }
        coefficients.source += dir_sign * flow_rate * face_velocity;
    }

    return coefficients;
}
