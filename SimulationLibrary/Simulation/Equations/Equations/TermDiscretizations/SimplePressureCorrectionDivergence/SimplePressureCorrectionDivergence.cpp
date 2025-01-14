#include "SimplePressureCorrectionDivergence.hpp"

#include "../../../../Meshing/Faces/Boundary/BoundaryFace.hpp"
#include "../../../../Meshing/Faces/Interior/InteriorFace.hpp"

SimplePressureCorrectionDivergence::SimplePressureCorrectionDivergence(Node *node) : TermDiscretization(node) {
}

Coefficients SimplePressureCorrectionDivergence::get_effects() const {
    Coefficients coefficients;

    for (int dir = 0; dir < direction_near_end; dir++) {
        Coefficients direction_coefficients;

        const Direction direction = static_cast<Direction>(dir);
        Face *face_direction = m_node->get_neighbouring_face(direction);
        const Orientation orientation = direction == Direction::West || direction == Direction::East
                                            ? Orientation::Horizontal
                                            : Orientation::Vertical;

        // Get the face velocity and density
        double velocity_face;
        double density_face;
        if (face_direction->get_face_type() == FaceType::Boundary) {
            const BoundaryFace *boundary_face_face = static_cast<BoundaryFace *>(face_direction);
            velocity_face = orientation == Orientation::Horizontal
                                ? boundary_face_face->get_field_value(Field::VelocityX)
                                : boundary_face_face->get_field_value(Field::VelocityY);
            density_face = boundary_face_face->get_density();
        } else {
            const InteriorFace *interior_face_face = static_cast<InteriorFace *>(face_direction);
            velocity_face = interior_face_face->get_velocity();
            density_face = interior_face_face->get_density();
        }

        const double area = orientation == Orientation::Horizontal ? m_node->get_dy() : m_node->get_dx();
        const double sign = direction == Direction::West || direction == Direction::South ? 1.0 : -1.0;

        direction_coefficients.source += sign * velocity_face * density_face * area;

        const EquationType equation_type =
            orientation == Orientation::Horizontal ? EquationType::MomentumX : EquationType::MomentumY;
        const double momentum_a_P = m_node->get_equation_coefficient(equation_type, CoefficientType::Center);

        if (face_direction->get_face_type() != FaceType::Boundary) {
            const double momentum_a_direction = m_node->get_neighbouring_node(direction)->get_equation_coefficient(
                equation_type, CoefficientType::Center);
            const double extra_a_W =
                0.5 * (1 / momentum_a_P + 1 / momentum_a_direction) * m_node->get_dt() * area * area * density_face;
            direction_coefficients.add_to_coefficient(direction, extra_a_W);
            direction_coefficients.center += extra_a_W;
        } else if (static_cast<BoundaryFace *>(face_direction)->get_boundary_type() == BoundaryType::FixedPressure) {
            const double extra = 0.5 * m_node->get_dt() * area * area * density_face / momentum_a_P;
            const Direction opposite_direction = get_opposite_direction(direction);
            direction_coefficients.add_to_coefficient(opposite_direction, -extra);
            direction_coefficients.center += extra;
        }

        coefficients += direction_coefficients;
    }

    return coefficients;
}
