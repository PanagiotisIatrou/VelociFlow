#include "../Node.hpp"

#include "../../Faces/Boundary/BoundaryFace.hpp"
#include "../../Faces/Interior/InteriorFace.hpp"

PressureCorrectionCoefficients::PressureCorrectionCoefficients(Node *node) {
    m_node = node;
}

void PressureCorrectionCoefficients::calculate_coefficients() {
    Face *face_w = m_node->get_neighbouring_face(Direction::West);
    Face *face_e = m_node->get_neighbouring_face(Direction::East);
    Face *face_s = m_node->get_neighbouring_face(Direction::South);
    Face *face_n = m_node->get_neighbouring_face(Direction::North);

    Coefficients coefficients;

    // Velocity/density w
    double velocity_w;
    double density_w;
    if (face_w->get_face_type() == FaceType::Boundary) {
        const BoundaryFace *boundary_face_w = static_cast<BoundaryFace *>(face_w);
        velocity_w = boundary_face_w->get_field_value(Field::VelocityX);
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
        velocity_e = boundary_face_e->get_field_value(Field::VelocityX);
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
        velocity_s = boundary_face_s->get_field_value(Field::VelocityY);
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
        velocity_n = boundary_face_n->get_field_value(Field::VelocityY);
        density_n = boundary_face_n->get_density();
    } else {
        const InteriorFace *interior_face_n = static_cast<InteriorFace *>(face_n);
        velocity_n = interior_face_n->get_velocity();
        density_n = interior_face_n->get_density();
    }

    // Calculate the mass imbalance (source)
    coefficients.source += -(
        velocity_e * density_e * m_node->get_dy()
        - velocity_w * density_w * m_node->get_dy()
        + velocity_n * density_n * m_node->get_dx()
        - velocity_s * density_s * m_node->get_dx()
        );

    const double momentum_x_a_P = m_node->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
    const double momentum_y_a_P = m_node->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::V);

    // a_W
    if (face_w->get_face_type() != FaceType::Boundary) {
        const double momentum_x_a_W = m_node->get_neighbouring_node(Direction::West)->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
        const double extra_a_W = 0.5 * (1 / momentum_x_a_P + 1 / momentum_x_a_W) * m_node->get_dt() * m_node->get_dy() * m_node->get_dy() * density_w;
        coefficients.west += extra_a_W;
        coefficients.center += extra_a_W;
    } else if (static_cast<BoundaryFace *>(face_w)->get_boundary_type() == BoundaryType::FixedPressure) {
        const double extra = 0.5 * m_node->get_dt() * m_node->get_dy() * m_node->get_dy() * density_w / momentum_x_a_P;
        coefficients.east -= extra;
        coefficients.center += extra;
    }

    // a_E
    if (face_e->get_face_type() != FaceType::Boundary) {
        const double momentum_x_a_E = m_node->get_neighbouring_node(Direction::East)->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
        const double extra_a_E = 0.5 * (1 / momentum_x_a_P + 1 / momentum_x_a_E) * m_node->get_dt() * m_node->get_dy() * m_node->get_dy() * density_e;
        coefficients.east += extra_a_E;
        coefficients.center += extra_a_E;
    } else if (static_cast<BoundaryFace *>(face_e)->get_boundary_type() == BoundaryType::FixedPressure) {
        const double extra = 0.5 * m_node->get_dt() * m_node->get_dy() * m_node->get_dy() * density_e / momentum_x_a_P;
        coefficients.west -= extra;
        coefficients.center += extra;
    }

    // a_S
    if (face_s->get_face_type() != FaceType::Boundary) {
        const double momentum_y_a_S = m_node->get_neighbouring_node(Direction::South)->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::V);
        const double extra_a_S = 0.5 * (1 / momentum_y_a_P + 1 / momentum_y_a_S) * m_node->get_dt() * m_node->get_dx() * m_node->get_dx() * density_s;
        coefficients.south += extra_a_S;
        coefficients.center += extra_a_S;
    } else if (static_cast<BoundaryFace *>(face_s)->get_boundary_type() == BoundaryType::FixedPressure) {
        const double extra = 0.5 * m_node->get_dt() * m_node->get_dx() * m_node->get_dx() * density_s / momentum_y_a_P;
        coefficients.north -= extra;
        coefficients.center += extra;
    }

    // a_N
    if (face_n->get_face_type() != FaceType::Boundary) {
        const double momentum_y_a_N = m_node->get_neighbouring_node(Direction::North)->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::V);
        const double extra_a_N = 0.5 * (1 / momentum_y_a_P + 1 / momentum_y_a_N) * m_node->get_dt() * m_node->get_dx() * m_node->get_dx() * density_n;
        coefficients.north += extra_a_N;
        coefficients.center += extra_a_N;
    } else if (static_cast<BoundaryFace *>(face_n)->get_boundary_type() == BoundaryType::FixedPressure) {
        const double extra = 0.5 * m_node->get_dt() * m_node->get_dx() * m_node->get_dx() * density_n / momentum_y_a_P;
        coefficients.south -= extra;
        coefficients.center += extra;
    }

    m_coefficients = coefficients;
}

Coefficients PressureCorrectionCoefficients::get_coefficients() const {
    return m_coefficients;
}

double PressureCorrectionCoefficients::get_coefficient(const CoefficientType type) const {
    return m_coefficients.get_coefficient(type);
}
