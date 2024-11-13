#include "Node.hpp"

#include "../../common.hpp"
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

    // Velocity w
    double velocity_w;
    if (face_w->get_face_type() == FaceType::Boundary) {
        velocity_w = static_cast<BoundaryFace *>(face_w)->get_velocity_u();
    } else {
        velocity_w = static_cast<InteriorFace *>(face_w)->get_velocity();
    }

    // Velocity e
    double velocity_e;
    if (face_e->get_face_type() == FaceType::Boundary) {
        velocity_e = static_cast<BoundaryFace *>(face_e)->get_velocity_u();
    } else {
        velocity_e = static_cast<InteriorFace *>(face_e)->get_velocity();
    }

    // Velocity s
    double velocity_s;
    if (face_s->get_face_type() == FaceType::Boundary) {
        velocity_s = static_cast<BoundaryFace *>(face_s)->get_velocity_v();
    } else {
        velocity_s = static_cast<InteriorFace *>(face_s)->get_velocity();
    }

    // Velocity n
    double velocity_n;
    if (face_n->get_face_type() == FaceType::Boundary) {
        velocity_n = static_cast<BoundaryFace *>(face_n)->get_velocity_v();
    } else {
        velocity_n = static_cast<InteriorFace *>(face_n)->get_velocity();
    }

    // Calculate the mass imbalance (source)
    source += -(velocity_e * m_dy - velocity_w * m_dy + velocity_n * m_dx - velocity_s * m_dx);

    const double momentum_u_a_P = get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
    const double momentum_v_a_P = get_momentum_coefficient(CoefficientType::Center, VelocityComponent::V);

    // a_W
    if (face_w->get_face_type() != FaceType::Boundary) {
        const double momentum_u_a_W = get_neighbouring_node(Direction::West)->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
        const double extra_a_W = 0.5 * (1 / momentum_u_a_P + 1 / momentum_u_a_W) * m_dy * m_dy;
        a_W += extra_a_W;
        a_P += extra_a_W;
    }

    // a_E
    if (face_e->get_face_type() != FaceType::Boundary) {
        const double momentum_u_a_E = get_neighbouring_node(Direction::East)->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
        const double extra_a_E = 0.5 * (1 / momentum_u_a_P + 1 / momentum_u_a_E) * m_dy * m_dy;
        a_E += extra_a_E;
        a_P += extra_a_E;
    }

    // a_S
    if (face_s->get_face_type() != FaceType::Boundary) {
        const double momentum_v_a_S = get_neighbouring_node(Direction::South)->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::V);
        const double extra_a_S = 0.5 * (1 / momentum_v_a_P + 1 / momentum_v_a_S) * m_dx * m_dx;
        a_S += extra_a_S;
        a_P += extra_a_S;
    }

    // a_N
    if (face_n->get_face_type() != FaceType::Boundary) {
        const double momentum_v_a_N = get_neighbouring_node(Direction::North)->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::V);
        const double extra_a_N = 0.5 * (1 / momentum_v_a_P + 1 / momentum_v_a_N) * m_dx * m_dx;
        a_N += extra_a_N;
        a_P += extra_a_N;
    }

    m_pressure_coefficients = {a_P, source, a_W, a_E, a_S, a_N};
}

std::array<double, 6> Node::get_pressure_coefficients() const {
    return m_pressure_coefficients;
}

double Node::get_pressure_coefficient(const CoefficientType type) const {
    return m_pressure_coefficients[static_cast<int>(type)];
}
