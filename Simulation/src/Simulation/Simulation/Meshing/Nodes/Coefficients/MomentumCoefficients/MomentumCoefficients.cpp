#include "MomentumCoefficients.hpp"

#include "../../Node.hpp"
#include "../../../../../common.hpp"
#include "../../../Faces/Boundary/BoundaryFace.hpp"


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

Coefficients MomentumCoefficients::get_convection_effects(const VelocityComponent velocity_component, const ConvectionSchemes convection_scheme) const {
    Coefficients coefficients;

    for (int dir = 0; dir < direction_near_end; dir++) {
        const Direction direction = static_cast<Direction>(dir);
        Coefficients direction_coefficients;
        switch (convection_scheme) {
            case ConvectionSchemes::Upwind: {
                direction_coefficients = get_upwind_effects(direction, velocity_component);
                break;
            }
            case ConvectionSchemes::CentralDifferencing: {
                direction_coefficients = get_central_differencing_effects(direction, velocity_component);
                break;
            }
            case ConvectionSchemes::QuickHayase: {
                direction_coefficients = get_quick_hayase_effects(direction, velocity_component);
                break;
            }
        }
        coefficients += direction_coefficients;
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
    const Coefficients convection_coefficients = get_convection_effects(velocity_component, ConvectionSchemes::QuickHayase);

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
