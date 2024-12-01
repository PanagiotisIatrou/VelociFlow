#include "MomentumCoefficients.hpp"

#include "../../Node.hpp"
#include "../../../../../common.hpp"
#include "../../../Faces/Boundary/BoundaryFace.hpp"
#include "../../../Faces/Interior/InteriorFace.hpp"


MomentumCoefficients::MomentumCoefficients(Node *node) {
    m_node = node;
}

Coefficients MomentumCoefficients::get_diffusion_effects(const VelocityComponent velocity_component) const {
    Coefficients coefficients;

    // a_W
    Face *face_w = m_node->get_neighbouring_face(Direction::West);
    if (face_w->get_face_type() != FaceType::Boundary) {
        const double extra_a_W = face_w->get_viscosity() * m_node->m_dt * m_node->m_dy / m_node->m_dx;
        coefficients.west += extra_a_W;
        coefficients.center += extra_a_W;
    } else {
        double face_velocity_w;
        BoundaryFace *boundary_face_w = static_cast<BoundaryFace *>(face_w);
        if (velocity_component == VelocityComponent::U) {
            face_velocity_w = boundary_face_w->get_velocity_u();
        } else {
            face_velocity_w = boundary_face_w->get_velocity_v();
        }
        double extra_a_P = 2.0 * face_w->get_viscosity() * m_node->m_dt * m_node->m_dy / m_node->m_dx;
        coefficients.center += extra_a_P;
        coefficients.source += face_velocity_w * extra_a_P;
    }

    // a_E
    Face *face_e = m_node->get_neighbouring_face(Direction::East);
    if (face_e->get_face_type() != FaceType::Boundary) {
        const double extra_a_E = face_e->get_viscosity() * m_node->m_dt * m_node->m_dy / m_node->m_dx;
        coefficients.east += extra_a_E;
        coefficients.center += extra_a_E;
    } else {
        double face_velocity_e;
        BoundaryFace *boundary_face_e = static_cast<BoundaryFace *>(face_e);
        if (velocity_component == VelocityComponent::U) {
            face_velocity_e = boundary_face_e->get_velocity_u();
        } else {
            face_velocity_e = boundary_face_e->get_velocity_v();
        }
        double extra_a_P = 2.0 * face_e->get_viscosity() * m_node->m_dt * m_node->m_dy / m_node->m_dx;
        coefficients.center += extra_a_P;
        coefficients.source += face_velocity_e * extra_a_P;
    }

    // a_S
    Face *face_s = m_node->get_neighbouring_face(Direction::South);
    if (face_s->get_face_type() != FaceType::Boundary) {
        const double extra_a_S = face_s->get_viscosity() * m_node->m_dt * m_node->m_dx / m_node->m_dy;
        coefficients.south += extra_a_S;
        coefficients.center += extra_a_S;
    } else {
        double face_velocity_s;
        BoundaryFace *boundary_face_s = static_cast<BoundaryFace *>(face_s);
        if (velocity_component == VelocityComponent::U) {
            face_velocity_s = boundary_face_s->get_velocity_u();
        } else {
            face_velocity_s = boundary_face_s->get_velocity_v();
        }
        double extra_a_P = 2.0 * face_s->get_viscosity() * m_node->m_dt * m_node->m_dx / m_node->m_dy;
        coefficients.center += extra_a_P;
        coefficients.source += face_velocity_s * extra_a_P;
    }

    // a_N
    Face *face_n = m_node->get_neighbouring_face(Direction::North);
    if (face_n->get_face_type() != FaceType::Boundary) {
        const double extra_a_N = face_n->get_viscosity() * m_node->m_dt * m_node->m_dx / m_node->m_dy;
        coefficients.north += extra_a_N;
        coefficients.center += extra_a_N;
    } else {
        double face_velocity_n;
        BoundaryFace *boundary_face_n = static_cast<BoundaryFace *>(face_n);
        if (velocity_component == VelocityComponent::U) {
            face_velocity_n = boundary_face_n->get_velocity_u();
        } else {
            face_velocity_n = boundary_face_n->get_velocity_v();
        }
        double extra_a_P = 2.0 * face_n->get_viscosity() * m_node->m_dt * m_node->m_dx / m_node->m_dy;
        coefficients.center += extra_a_P;
        coefficients.source += face_velocity_n * extra_a_P;
    }

    return coefficients;
}

Coefficients MomentumCoefficients::get_convection_effects(const VelocityComponent velocity_component) const {
    Coefficients coefficients;

    // Upwind
    // for (int dir = 0; dir < direction_near_end; dir++) {
    //     const Direction direction = static_cast<Direction>(dir);
    //     Face *face = m_node->get_neighbouring_face(direction);
    //     const double area = direction == Direction::West || direction == Direction::East ? m_node->m_dy : m_node->m_dx;
    //     const double flow_rate = face->get_density() * m_node->m_dt * area * face->get_normal_velocity();
    //     if (face->get_face_type() != FaceType::Boundary) {
    //         const int dir_sign = direction == Direction::West || direction == Direction::South ? 1 : -1;
    //         const double extra = std::max(dir_sign * flow_rate, 0.0);
    //         coefficients.add_to_coefficient(direction, extra);
    //         coefficients.center += extra - dir_sign * flow_rate;
    //     } else {
    //         double face_velocity;
    //         const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
    //         if (velocity_component == VelocityComponent::U) {
    //             face_velocity = boundary_face->get_velocity_u();
    //         } else {
    //             face_velocity = boundary_face->get_velocity_v();
    //         }
    //         coefficients.source += flow_rate * face_velocity;
    //     }
    // }

    // Central differencing
    // for (int dir = 0; dir < direction_near_end; dir++) {
    //     const Direction direction = static_cast<Direction>(dir);
    //     Face *face = m_node->get_neighbouring_face(direction);
    //     const double area = direction == Direction::West || direction == Direction::East ? m_node->m_dy : m_node->m_dx;
    //     const double flow_rate = face->get_density() * m_node->m_dt * area * face->get_normal_velocity();
    //     if (face->get_face_type() != FaceType::Boundary) {
    //         const int dir_sign = direction == Direction::West || direction == Direction::South ? 1 : -1;
    //         const double extra = 0.5 * dir_sign * flow_rate;
    //         coefficients.add_to_coefficient(direction, extra);
    //         coefficients.center += extra - dir_sign * flow_rate;
    //     } else {
    //         double face_velocity;
    //         const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
    //         if (velocity_component == VelocityComponent::U) {
    //             face_velocity = boundary_face->get_velocity_u();
    //         } else {
    //             face_velocity = boundary_face->get_velocity_v();
    //         }
    //         coefficients.source += flow_rate * face_velocity;
    //     }
    // }

    // Linear upwind
    // for (int dir = 0; dir < direction_near_end; dir++) {
    //     const Direction direction = static_cast<Direction>(dir);
    //     Face *face = m_node->get_neighbouring_face(direction);
    //     const double area = direction == Direction::West || direction == Direction::East ? m_node->m_dy : m_node->m_dx;
    //     const double flow_rate = face->get_density() * m_node->m_dt * area * face->get_normal_velocity();
    //     if (face->get_face_type() != FaceType::Boundary) {
    //         const Direction opposite_direction = get_opposite_direction(direction);
    //         Face *opposite_face = m_node->get_neighbouring_face(opposite_direction);
    //         const double opposite_flow_rate = opposite_face->get_density() * m_node->m_dt * area * opposite_face->get_normal_velocity();
    //         const double dir_sign = direction == Direction::West || direction == Direction::South ? 1.0 : -1.0;
    //         const double extra = 1.5 * std::max(dir_sign * flow_rate, 0.0)
    //                              + 0.5 * std::max(-1.0 * dir_sign * opposite_flow_rate, 0.0)
    //                              + dir_sign * 0.5 * opposite_flow_rate;
    //         const double extra_extended = -0.5 * std::max(dir_sign * flow_rate, 0.0);
    //         coefficients.add_to_coefficient(direction, extra);
    //         coefficients.center += extra - dir_sign * flow_rate;
    //
    //         const Direction extended_direction = extend_direction(direction);
    //         const Face *furthest_face = m_node->get_neighbouring_face(extended_direction);
    //         if (furthest_face->get_face_type() != FaceType::Boundary) {
    //             coefficients.add_to_coefficient(extended_direction, extra_extended);
    //             coefficients.center += extra_extended;
    //         } else {
    //             double face_velocity;
    //             double next_node_velocity;
    //             const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
    //             const Node *next_node = m_node->get_neighbouring_node(direction);
    //             if (velocity_component == VelocityComponent::U) {
    //                 face_velocity = boundary_face->get_velocity_u();
    //                 next_node_velocity = next_node->get_velocity_u();
    //             } else {
    //                 face_velocity = boundary_face->get_velocity_v();
    //                 next_node_velocity = next_node->get_velocity_v();
    //             }
    //             coefficients.source += extra_extended * (2 * face_velocity - next_node_velocity);
    //         }
    //     } else {
    //         double face_velocity;
    //         const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
    //         if (velocity_component == VelocityComponent::U) {
    //             face_velocity = boundary_face->get_velocity_u();
    //         } else {
    //             face_velocity = boundary_face->get_velocity_v();
    //         }
    //         coefficients.source += flow_rate * face_velocity;
    //     }
    // }

    // Hayase QUICK
    for (int dir = 0; dir < direction_near_end; dir++) {
        const Direction direction = static_cast<Direction>(dir);
        Face *face = m_node->get_neighbouring_face(direction);
        const double area = direction == Direction::West || direction == Direction::East ? m_node->m_dy : m_node->m_dx;
        const double flow_rate = face->get_density() * m_node->m_dt * area * face->get_normal_velocity();
        if (face->get_face_type() != FaceType::Boundary) {
            const double dir_sign = direction == Direction::West || direction == Direction::South ? 1.0 : -1.0;
            const double alpha = flow_rate > 0.0 ? 1.0 : 0.0;
            const double extra = alpha * flow_rate - (direction == Direction::East || direction == Direction::North ? flow_rate : 0);
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
            Direction opposite_direction = get_opposite_direction(direction);
            Face *opposite_direction_face = m_node->get_neighbouring_face(opposite_direction);
            double opposite_node_value;
            if (opposite_direction_face->get_face_type() == FaceType::Interior) {
                Node *opposite_node = m_node->get_neighbouring_node(opposite_direction);
                if (velocity_component == VelocityComponent::U) {
                    opposite_node_value = opposite_node->get_velocity_u();
                } else {
                    opposite_node_value = opposite_node->get_velocity_v();
                }
            } else {
                // TODO: extrapolate or mirror
                double boundary_value;
                if (velocity_component == VelocityComponent::U) {
                    boundary_value = static_cast<BoundaryFace *>(opposite_direction_face)->get_velocity_u();
                } else {
                    boundary_value = static_cast<BoundaryFace *>(opposite_direction_face)->get_velocity_v();
                }
                opposite_node_value = 2 * boundary_value - node_value;
            }

            // Get direction node value
            double direction_node_value;
            Node *direction_node = m_node->get_neighbouring_node(direction);
            if (velocity_component == VelocityComponent::U) {
                direction_node_value = direction_node->get_velocity_u();
            } else {
                direction_node_value = direction_node->get_velocity_v();
            }

            // Get extended node value
            Direction extended_direction = extend_direction(direction);
            Face *extended_direction_face = m_node->get_neighbouring_face(extended_direction);
            double extended_node_value;
            if (extended_direction_face->get_face_type() == FaceType::Interior) {
                Node *extended_node = m_node->get_neighbouring_node(extended_direction);
                if (velocity_component == VelocityComponent::U) {
                    extended_node_value = extended_node->get_velocity_u();
                } else {
                    extended_node_value = extended_node->get_velocity_v();
                }
            } else {
                // Mirror node
                double boundary_value;
                BoundaryFace *boundary_face = static_cast<BoundaryFace *>(extended_direction_face);
                if (velocity_component == VelocityComponent::U) {
                    boundary_value = boundary_face->get_velocity_u();
                } else {
                    boundary_value = boundary_face->get_velocity_v();
                }

                extended_node_value = 2 * boundary_value - direction_node_value;
            }

            // Source
            double fac1 = direction == Direction::West || direction == Direction::South ? alpha : 1 - alpha;
            double fac2 = direction == Direction::West || direction == Direction::South ? 1 - alpha : alpha;
            coefficients.source += dir_sign * (1.0 / 8.0) * fac1 * flow_rate * (3 * node_value - 2 * direction_node_value - extended_node_value);
            coefficients.source += dir_sign * (1.0 / 8.0) * fac2 * flow_rate * (3 * direction_node_value - 2 * node_value - opposite_node_value);
        } else {
            double face_velocity;
            const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
            if (velocity_component == VelocityComponent::U) {
                face_velocity = boundary_face->get_velocity_u();
            } else {
                face_velocity = boundary_face->get_velocity_v();
            }
            coefficients.source += flow_rate * face_velocity;
        }
    }

    return coefficients;
}

Coefficients MomentumCoefficients::get_pressure_effects(const VelocityComponent velocity_component) const {
    Coefficients coefficients;

    if (velocity_component == VelocityComponent::U) {
        const Face *face_w = m_node->get_neighbouring_face(Direction::West);
        const Face *face_e = m_node->get_neighbouring_face(Direction::East);

        coefficients.source += m_node->m_dt * m_node->m_dy * (face_w->get_pressure() - face_e->get_pressure());
    } else {
        const Face *face_s = m_node->get_neighbouring_face(Direction::South);
        const Face *face_n = m_node->get_neighbouring_face(Direction::North);

        coefficients.source += m_node->m_dt * m_node->m_dx * (face_s->get_pressure() - face_n->get_pressure());
    }

    return coefficients;
}

Coefficients MomentumCoefficients::get_time_effects(const VelocityComponent velocity_component) const {
    Coefficients coefficients;

    double extra = m_node->m_density * m_node->m_dx * m_node->m_dy;

    coefficients.center += extra;

    if (velocity_component == VelocityComponent::U) {
        extra *= m_node->get_previous_timestep_velocity_u();
    } else {
        extra *= m_node->get_previous_timestep_velocity_v();
    }
    coefficients.source += extra;

    return coefficients;
}

void MomentumCoefficients::calculate_momentum_coefficients(const VelocityComponent velocity_component,
                                                           const SimulationType simulation_type) {
    // Diffusion
    const Coefficients diffusion_coefficients = get_diffusion_effects(velocity_component);

    // Convection
    const Coefficients convection_coefficients = get_convection_effects(velocity_component);

    // Pressure
    const Coefficients pressure_coefficients = get_pressure_effects(velocity_component);

    // Time
    Coefficients time_coefficients = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    if (simulation_type == SimulationType::Unsteady) {
        time_coefficients = get_time_effects(velocity_component);
    }

    // Sum the coefficients
    Coefficients coefficients = diffusion_coefficients + convection_coefficients + pressure_coefficients +
                                time_coefficients;

    if (velocity_component == VelocityComponent::U) {
        // Under-relaxation
        coefficients.center /= velocity_u_relaxation;
        coefficients.source += (1 - velocity_u_relaxation) * coefficients.center * m_node->get_velocity_u();

        m_momentum_u_coefficients = coefficients;
    } else {
        // Under-relaxation
        coefficients.center /= velocity_v_relaxation;
        coefficients.source += (1 - velocity_v_relaxation) * coefficients.center * m_node->get_velocity_v();

        m_momentum_v_coefficients = coefficients;
    }
}

double MomentumCoefficients::get_momentum_coefficient(const CoefficientType type,
                                                      const VelocityComponent velocity_component) const {
    if (velocity_component == VelocityComponent::U) {
        return m_momentum_u_coefficients.get_coefficient(type);
    } else {
        return m_momentum_v_coefficients.get_coefficient(type);
    }
}

Coefficients MomentumCoefficients::get_momentum_coefficients(const VelocityComponent velocity_component) const {
    if (velocity_component == VelocityComponent::U) {
        return m_momentum_u_coefficients;
    } else {
        return m_momentum_v_coefficients;
    }
}
