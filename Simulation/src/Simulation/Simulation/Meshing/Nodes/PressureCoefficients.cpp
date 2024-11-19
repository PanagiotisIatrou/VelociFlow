#include "Node.hpp"

#include "../Faces/Boundary/BoundaryFace.hpp"
#include "../Faces/Interior/InteriorFace.hpp"

void Node::calculate_pressure_coefficients() {
    Face *face_w = get_neighbouring_face(Direction::West);
    Face *face_e = get_neighbouring_face(Direction::East);
    Face *face_s = get_neighbouring_face(Direction::South);
    Face *face_n = get_neighbouring_face(Direction::North);

    double source = 0.0;
    double a_P = 0.0;
    double a_W = 0.0;
    double a_E = 0.0;
    double a_S = 0.0;
    double a_N = 0.0;

    // Velocity/density w
    double velocity_w;
    double density_w;
    if (face_w->get_face_type() == FaceType::Boundary) {
        const BoundaryFace *boundary_face_w = static_cast<BoundaryFace *>(face_w);
        velocity_w = boundary_face_w->get_velocity_u();
        density_w = boundary_face_w->get_density();
    } else {
        const InteriorFace *interior_face_w = static_cast<InteriorFace *>(face_w);
        velocity_w = interior_face_w->get_velocity();
        density_w = interior_face_w->get_density();
    }

    // Velocity/density e
    double velocity_e;
    double density_e;
    if (face_e->get_face_type() == FaceType::Boundary) {
        const BoundaryFace *boundary_face_e = static_cast<BoundaryFace *>(face_e);
        velocity_e = boundary_face_e->get_velocity_u();
        density_e = boundary_face_e->get_density();
    } else {
        const InteriorFace *interior_face_e = static_cast<InteriorFace *>(face_e);
        velocity_e = interior_face_e->get_velocity();
        density_e = interior_face_e->get_density();
    }

    // Velocity/density s
    double velocity_s;
    double density_s;
    if (face_s->get_face_type() == FaceType::Boundary) {
        const BoundaryFace *boundary_face_s = static_cast<BoundaryFace *>(face_s);
        velocity_s = boundary_face_s->get_velocity_v();
        density_s = boundary_face_s->get_density();
    } else {
        const InteriorFace *interior_face_s = static_cast<InteriorFace *>(face_s);
        velocity_s = interior_face_s->get_velocity();
        density_s = interior_face_s->get_density();
    }

    // Velocity/density n
    double velocity_n;
    double density_n;
    if (face_n->get_face_type() == FaceType::Boundary) {
        const BoundaryFace *boundary_face_n = static_cast<BoundaryFace *>(face_n);
        velocity_n = boundary_face_n->get_velocity_v();
        density_n = boundary_face_n->get_density();
    } else {
        const InteriorFace *interior_face_n = static_cast<InteriorFace *>(face_n);
        velocity_n = interior_face_n->get_velocity();
        density_n = interior_face_n->get_density();
    }

    // Calculate the mass imbalance (source)
    source += -(
        velocity_e * density_e * m_dy
        - velocity_w * density_w * m_dy
        + velocity_n * density_n * m_dx
        - velocity_s * density_s * m_dx
        );

    const double momentum_u_a_P = get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
    const double momentum_v_a_P = get_momentum_coefficient(CoefficientType::Center, VelocityComponent::V);

    // a_W
    if (face_w->get_face_type() != FaceType::Boundary) {
        const double momentum_u_a_W = get_neighbouring_node(Direction::West)->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
        const double extra_a_W = 0.5 * (1 / momentum_u_a_P + 1 / momentum_u_a_W) * m_dt * m_dy * m_dy * density_w;
        a_W += extra_a_W;
        a_P += extra_a_W;
    } else if (static_cast<BoundaryFace *>(face_w)->get_boundary_type() == BoundaryType::FixedPressure) {
        const double extra = 0.5 * m_dt * m_dy * m_dy * density_w / momentum_u_a_P;
        a_E -= extra;
        a_P += extra;
    }

    // a_E
    if (face_e->get_face_type() != FaceType::Boundary) {
        const double momentum_u_a_E = get_neighbouring_node(Direction::East)->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
        const double extra_a_E = 0.5 * (1 / momentum_u_a_P + 1 / momentum_u_a_E) * m_dt * m_dy * m_dy * density_e;
        a_E += extra_a_E;
        a_P += extra_a_E;
    } else if (static_cast<BoundaryFace *>(face_e)->get_boundary_type() == BoundaryType::FixedPressure) {
        const double extra = 0.5 * m_dt * m_dy * m_dy * density_e / momentum_u_a_P;
        a_W -= extra;
        a_P += extra;
    }

    // a_S
    if (face_s->get_face_type() != FaceType::Boundary) {
        const double momentum_v_a_S = get_neighbouring_node(Direction::South)->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::V);
        const double extra_a_S = 0.5 * (1 / momentum_v_a_P + 1 / momentum_v_a_S) * m_dt * m_dx * m_dx * density_s;
        a_S += extra_a_S;
        a_P += extra_a_S;
    } else if (static_cast<BoundaryFace *>(face_s)->get_boundary_type() == BoundaryType::FixedPressure) {
        const double extra = 0.5 * m_dt * m_dx * m_dx * density_s / momentum_v_a_P;
        a_N -= extra;
        a_P += extra;
    }

    // a_N
    if (face_n->get_face_type() != FaceType::Boundary) {
        const double momentum_v_a_N = get_neighbouring_node(Direction::North)->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::V);
        const double extra_a_N = 0.5 * (1 / momentum_v_a_P + 1 / momentum_v_a_N) * m_dt * m_dx * m_dx * density_n;
        a_N += extra_a_N;
        a_P += extra_a_N;
    } else if (static_cast<BoundaryFace *>(face_n)->get_boundary_type() == BoundaryType::FixedPressure) {
        const double extra = 0.5 * m_dt * m_dx * m_dx * density_n / momentum_v_a_P;
        a_S -= extra;
        a_P += extra;
    }

    m_pressure_coefficients = {a_P, source, a_W, a_E, a_S, a_N};
}

std::array<double, 6> Node::get_pressure_coefficients() const {
    return m_pressure_coefficients;
}

double Node::get_pressure_coefficient(const CoefficientType type) const {
    return m_pressure_coefficients[static_cast<int>(type)];
}
