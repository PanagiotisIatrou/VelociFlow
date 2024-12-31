#include "DiffusionCoefficients.hpp"

#include <iostream>

#include "../../../Faces/Boundary/BoundaryFace.hpp"

class Face;

DiffusionCoefficients::DiffusionCoefficients(Node *node) {
    m_node = node;
}

Coefficients DiffusionCoefficients::get_diffusion_effects(const Field field, const DiffusionSchemes diffusion_scheme) const {
    Coefficients coefficients;

    for (int dir = 0; dir < direction_near_end; dir++) {
        const Direction direction = static_cast<Direction>(dir);
        Coefficients direction_coefficients;
        switch (diffusion_scheme) {
            case DiffusionSchemes::CentralDifferencing: {
                direction_coefficients = get_central_differencing_diffusion_effects(direction, field);
                break;
            }
        }
        coefficients += direction_coefficients;
    }

    return coefficients;
}

Coefficients DiffusionCoefficients::get_central_differencing_diffusion_effects(const Direction direction, const Field field) const {
    Coefficients coefficients;

    Face *face = m_node->get_neighbouring_face(direction);
    double flux = face->get_viscosity() * m_node->get_dt();
    if (direction == Direction::West || direction == Direction::East) {
        flux *= m_node->get_dy() / m_node->get_dx();
    } else {
        flux *= m_node->get_dx() / m_node->get_dy();
    }
    if (face->get_face_type() != FaceType::Boundary) {
        coefficients.add_to_coefficient(direction, flux);
        coefficients.center += flux;
    } else {
        const BoundaryFace *boundary_face = static_cast<BoundaryFace *>(face);
        const double face_value = boundary_face->get_field_value(field);
        const double extra = 2.0 * flux;
        coefficients.center += extra;
        coefficients.source += face_value * extra;
    }

    return coefficients;
}
