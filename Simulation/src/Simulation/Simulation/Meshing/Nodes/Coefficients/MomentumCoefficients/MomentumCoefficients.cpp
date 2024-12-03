#include "MomentumCoefficients.hpp"

#include "../../Node.hpp"
#include "../../../../../common.hpp"
#include "../../../Faces/Boundary/BoundaryFace.hpp"


MomentumCoefficients::MomentumCoefficients(Node *node) {
    m_node = node;
}

Coefficients MomentumCoefficients::get_diffusion_effects(const VelocityComponent velocity_component, const DiffusionSchemes diffusion_scheme) const {
    Coefficients coefficients;

    for (int dir = 0; dir < direction_near_end; dir++) {
        const Direction direction = static_cast<Direction>(dir);
        Coefficients direction_coefficients;
        switch (diffusion_scheme) {
            case DiffusionSchemes::CentralDifferencing: {
                direction_coefficients = get_central_differencing_diffusion_effects(direction, velocity_component);
                break;
            }
        }
        coefficients += direction_coefficients;
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
    const Coefficients diffusion_coefficients = get_diffusion_effects(velocity_component, DiffusionSchemes::CentralDifferencing);

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
