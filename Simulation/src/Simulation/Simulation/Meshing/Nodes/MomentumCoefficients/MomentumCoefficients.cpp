#include <iostream>

#include "../Node.hpp"
#include "../../../../common.hpp"
#include "../../../Simulation.hpp"

#include "../../Faces/Boundary/BoundaryFace.hpp"
#include "../../Faces/Interior/InteriorFace.hpp"

MomentumCoefficients::MomentumCoefficients(Node *node) {
    m_node = node;
    m_discretization_schemes = std::make_unique<DiscretizationSchemes>(node);
}

std::array<double, 6> MomentumCoefficients::get_diffusion_effects(const VelocityComponent velocity_component) const {
    double source = 0.0;
    double a_P = 0.0;
    double a_W = 0.0;
    double a_E = 0.0;
    double a_S = 0.0;
    double a_N = 0.0;

    // a_W
    Face *face_w = m_node->get_neighbouring_face(Direction::West);
    if (face_w->get_face_type() != FaceType::Boundary) {
        const double extra_a_W = face_w->get_viscosity() * m_node->m_dt * m_node->m_dy / m_node->m_dx;
        a_W += extra_a_W;
        a_P += extra_a_W;
    } else {
        double face_velocity_w;
        BoundaryFace *boundary_face_w = static_cast<BoundaryFace *>(face_w);
        if (velocity_component == VelocityComponent::U) {
            face_velocity_w = boundary_face_w->get_velocity_u();
        } else {
            face_velocity_w = boundary_face_w->get_velocity_v();
        }
        double extra_a_P = 2.0 * face_w->get_viscosity() * m_node->m_dt * m_node->m_dy / m_node->m_dx;
        a_P += extra_a_P;
        source += face_velocity_w * extra_a_P;
    }

    // a_E
    Face *face_e = m_node->get_neighbouring_face(Direction::East);
    if (face_e->get_face_type() != FaceType::Boundary) {
        const double extra_a_E = face_e->get_viscosity() * m_node->m_dt * m_node->m_dy / m_node->m_dx;
        a_E += extra_a_E;
        a_P += extra_a_E;
    } else {
        double face_velocity_e;
        BoundaryFace *boundary_face_e = static_cast<BoundaryFace *>(face_e);
        if (velocity_component == VelocityComponent::U) {
            face_velocity_e = boundary_face_e->get_velocity_u();
        } else {
            face_velocity_e = boundary_face_e->get_velocity_v();
        }
        double extra_a_P = 2.0 * face_e->get_viscosity() * m_node->m_dt * m_node->m_dy / m_node->m_dx;
        a_P += extra_a_P;
        source += face_velocity_e * extra_a_P;
    }

    // a_S
    Face *face_s = m_node->get_neighbouring_face(Direction::South);
    if (face_s->get_face_type() != FaceType::Boundary) {
        const double extra_a_S = face_s->get_viscosity() * m_node->m_dt * m_node->m_dx / m_node->m_dy;
        a_S += extra_a_S;
        a_P += extra_a_S;
    } else {
        double face_velocity_s;
        BoundaryFace *boundary_face_s = static_cast<BoundaryFace *>(face_s);
        if (velocity_component == VelocityComponent::U) {
            face_velocity_s = boundary_face_s->get_velocity_u();
        } else {
            face_velocity_s = boundary_face_s->get_velocity_v();
        }
        double extra_a_P = 2.0 * face_s->get_viscosity() * m_node->m_dt * m_node->m_dx / m_node->m_dy;
        a_P += extra_a_P;
        source += face_velocity_s * extra_a_P;
    }

    // a_N
    Face *face_n = m_node->get_neighbouring_face(Direction::North);
    if (face_n->get_face_type() != FaceType::Boundary) {
        const double extra_a_N = face_n->get_viscosity() * m_node->m_dt * m_node->m_dx / m_node->m_dy;
        a_N += extra_a_N;
        a_P += extra_a_N;
    } else {
        double face_velocity_n;
        BoundaryFace *boundary_face_n = static_cast<BoundaryFace *>(face_n);
        if (velocity_component == VelocityComponent::U) {
            face_velocity_n = boundary_face_n->get_velocity_u();
        } else {
            face_velocity_n = boundary_face_n->get_velocity_v();
        }
        double extra_a_P = 2.0 * face_n->get_viscosity() * m_node->m_dt * m_node->m_dx / m_node->m_dy;
        a_P += extra_a_P;
        source += face_velocity_n * extra_a_P;
    }

    return {a_P, source, a_W, a_E, a_S, a_N};
}

std::array<double, 6> MomentumCoefficients::get_convection_effects(const VelocityComponent velocity_component) const {
    double source = 0.0;
    double a_P = 0.0;
    double a_W = 0.0;
    double a_E = 0.0;
    double a_S = 0.0;
    double a_N = 0.0;

    SchemeCoefficients west = m_discretization_schemes->convection(Direction::West, Scheme::Upwind, velocity_component);
    a_W += west.node0;
    a_P += -west.node1;
    source += -west.source;
    SchemeCoefficients east = m_discretization_schemes->convection(Direction::East, Scheme::Upwind, velocity_component);
    a_P += east.node0;
    a_E += -east.node1;
    source += -east.source;
    SchemeCoefficients south = m_discretization_schemes->convection(Direction::South, Scheme::Upwind, velocity_component);
    a_S += south.node0;
    a_P += -south.node1;
    source += -south.source;
    SchemeCoefficients north = m_discretization_schemes->convection(Direction::North, Scheme::Upwind, velocity_component);
    a_P += north.node0;
    a_N += -north.node1;
    source += -north.source;

    return {a_P, source, a_W, a_E, a_S, a_N};

    ///////////////////////////////////

    // // a_W
    // Face *face_w = m_node->get_neighbouring_face(Direction::West);
    // const double density_w = face_w->get_density();
    // if (face_w->get_face_type() != FaceType::Boundary) {
    //     const double face_velocity_u_w = static_cast<InteriorFace *>(face_w)->get_velocity();
    //     const double extra_a_W = density_w * m_node->m_dt * m_node->m_dy * std::max(face_velocity_u_w, 0.0);
    //     a_W += extra_a_W;
    //     a_P += extra_a_W - density_w * m_node->m_dt * m_node->m_dy * face_velocity_u_w;
    // } else {
    //     double face_velocity_w;
    //     BoundaryFace *boundary_face_w = static_cast<BoundaryFace *>(face_w);
    //     if (velocity_component == VelocityComponent::U) {
    //         face_velocity_w = boundary_face_w->get_velocity_u();
    //     } else {
    //         face_velocity_w = boundary_face_w->get_velocity_v();
    //     }
    //     source += density_w * m_node->m_dt * m_node->m_dy * boundary_face_w->get_velocity_u() * face_velocity_w;
    // }
    //
    // // a_E
    // Face *face_e = m_node->get_neighbouring_face(Direction::East);
    // const double density_e = face_e->get_density();
    // if (face_e->get_face_type() != FaceType::Boundary) {
    //     const double face_velocity_u_e = static_cast<InteriorFace *>(face_e)->get_velocity();
    //     const double extra_a_E = density_e * m_node->m_dt * m_node->m_dy * std::max(-face_velocity_u_e, 0.0);
    //     a_E += extra_a_E;
    //     a_P += extra_a_E + density_e * m_node->m_dt * m_node->m_dy * face_velocity_u_e;
    // } else {
    //     double face_velocity_e;
    //     BoundaryFace *boundary_face_e = static_cast<BoundaryFace *>(face_e);
    //     if (velocity_component == VelocityComponent::U) {
    //         face_velocity_e = boundary_face_e->get_velocity_u();
    //     } else {
    //         face_velocity_e = boundary_face_e->get_velocity_v();
    //     }
    //     source += -density_e * m_node->m_dt * m_node->m_dy * boundary_face_e->get_velocity_u() * face_velocity_e;
    // }
    //
    // // a_S
    // Face *face_s = m_node->get_neighbouring_face(Direction::South);
    // const double density_s = face_s->get_density();
    // if (face_s->get_face_type() != FaceType::Boundary) {
    //     const double face_velocity_v_s = static_cast<InteriorFace *>(face_s)->get_velocity();
    //     const double extra_a_S = density_s * m_node->m_dt * m_node->m_dx * std::max(face_velocity_v_s, 0.0);
    //     a_S += extra_a_S;
    //     a_P += extra_a_S - density_s * m_node->m_dt * m_node->m_dx * face_velocity_v_s;
    // } else {
    //     double face_velocity_s;
    //     BoundaryFace *boundary_face_s = static_cast<BoundaryFace *>(face_s);
    //     if (velocity_component == VelocityComponent::U) {
    //         face_velocity_s = boundary_face_s->get_velocity_u();
    //     } else {
    //         face_velocity_s = boundary_face_s->get_velocity_v();
    //     }
    //     source += density_s * m_node->m_dt * m_node->m_dx * boundary_face_s->get_velocity_v() * face_velocity_s;
    // }
    //
    // // a_N
    // Face *face_n = m_node->get_neighbouring_face(Direction::North);
    // const double density_n = face_n->get_density();
    // if (face_n->get_face_type() != FaceType::Boundary) {
    //     const double face_velocity_v_n = static_cast<InteriorFace *>(face_n)->get_velocity();
    //     const double extra_a_N = density_n * m_node->m_dt * m_node->m_dx * std::max(-face_velocity_v_n, 0.0);
    //     a_N += extra_a_N;
    //     a_P += extra_a_N + density_n * m_node->m_dt * m_node->m_dx * face_velocity_v_n;
    // } else {
    //     double face_velocity_n;
    //     BoundaryFace *boundary_face_n = static_cast<BoundaryFace *>(face_n);
    //     if (velocity_component == VelocityComponent::U) {
    //         face_velocity_n = boundary_face_n->get_velocity_u();
    //     } else {
    //         face_velocity_n = boundary_face_n->get_velocity_v();
    //     }
    //     source += -density_n * m_node->m_dt * m_node->m_dx * boundary_face_n->get_velocity_v() * face_velocity_n;
    // }
    //
    // return {a_P, source, a_W, a_E, a_S, a_N};
}

std::array<double, 6> MomentumCoefficients::get_pressure_effects(const VelocityComponent velocity_component) const {
    double source = 0.0;
    double a_P = 0.0;
    double a_W = 0.0;
    double a_E = 0.0;
    double a_S = 0.0;
    double a_N = 0.0;

    if (velocity_component == VelocityComponent::U) {
        Face *face_w = m_node->get_neighbouring_face(Direction::West);
        Face *face_e = m_node->get_neighbouring_face(Direction::East);

        source += m_node->m_dt * m_node->m_dy * (face_w->get_pressure() - face_e->get_pressure());
    } else {
        Face *face_s = m_node->get_neighbouring_face(Direction::South);
        Face *face_n = m_node->get_neighbouring_face(Direction::North);

        source += m_node->m_dt * m_node->m_dx * (face_s->get_pressure() - face_n->get_pressure());
    }

    return {a_P, source, a_W, a_E, a_S, a_N};
}

std::array<double, 6> MomentumCoefficients::get_time_effects(const VelocityComponent velocity_component) const {
    double source = 0.0;
    double a_P = 0.0;
    double a_W = 0.0;
    double a_E = 0.0;
    double a_S = 0.0;
    double a_N = 0.0;

    double extra = m_node->m_density * m_node->m_dx * m_node->m_dy;

    a_P += extra;

    if (velocity_component == VelocityComponent::U) {
        extra *= m_node->get_previous_timestep_velocity_u();
    } else {
        extra *= m_node->get_previous_timestep_velocity_v();
    }
    source += extra;

    return {a_P, source, a_W, a_E, a_S, a_N};
}

void MomentumCoefficients::calculate_momentum_coefficients(const VelocityComponent velocity_component,
                                           const SimulationType simulation_type) {
    // Diffusion
    const std::array<double, 6> diffusion_coefficients = get_diffusion_effects(velocity_component);

    // Convection
    const std::array<double, 6> convection_coefficients = get_convection_effects(velocity_component);

    // Pressure
    const std::array<double, 6> pressure_coefficients = get_pressure_effects(velocity_component);

    // Time
    std::array<double, 6> time_coefficients = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    if (simulation_type == SimulationType::Unsteady) {
        time_coefficients = get_time_effects(velocity_component);
    }

    // Sum the coefficients
    double a_P = diffusion_coefficients[0] + convection_coefficients[0] + pressure_coefficients[0] + time_coefficients[
                     0];
    double source = diffusion_coefficients[1] + convection_coefficients[1] + pressure_coefficients[1] +
                    time_coefficients[1];
    const double a_W = diffusion_coefficients[2] + convection_coefficients[2] + pressure_coefficients[2] +
                       time_coefficients[2];
    const double a_E = diffusion_coefficients[3] + convection_coefficients[3] + pressure_coefficients[3] +
                       time_coefficients[3];
    const double a_S = diffusion_coefficients[4] + convection_coefficients[4] + pressure_coefficients[4] +
                       time_coefficients[4];
    const double a_N = diffusion_coefficients[5] + convection_coefficients[5] + pressure_coefficients[5] +
                       time_coefficients[5];

    if (velocity_component == VelocityComponent::U) {
        // Under-relaxation
        a_P /= velocity_u_relaxation;
        source += (1 - velocity_u_relaxation) * a_P * m_node->get_velocity_u();

        m_momentum_u_coefficients = {a_P, source, a_W, a_E, a_S, a_N};
    } else {
        // Under-relaxation
        a_P /= velocity_v_relaxation;
        source += (1 - velocity_v_relaxation) * a_P * m_node->get_velocity_v();

        m_momentum_v_coefficients = {a_P, source, a_W, a_E, a_S, a_N};
    }
}

double MomentumCoefficients::get_momentum_coefficient(const CoefficientType type, const VelocityComponent velocity_component) const {
    if (velocity_component == VelocityComponent::U) {
        return m_momentum_u_coefficients[static_cast<int>(type)];
    } else {
        return m_momentum_v_coefficients[static_cast<int>(type)];
    }
}

std::array<double, 6> MomentumCoefficients::get_momentum_coefficients(const VelocityComponent velocity_component) const {
    if (velocity_component == VelocityComponent::U) {
        return m_momentum_u_coefficients;
    } else {
        return m_momentum_v_coefficients;
    }
}
