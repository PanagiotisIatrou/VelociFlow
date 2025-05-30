#include "Diffusion.hpp"

#include "../../../../Meshing/Faces/Boundary/BoundaryFace.hpp"

Diffusion::Diffusion(Node *node, const Field field, const DiffusionSchemes diffusion_scheme)
    : TermDiscretization(node) {
    m_diffusion_scheme = diffusion_scheme;
    m_field = field;
}

Coefficients Diffusion::get_effects() const {
    Coefficients coefficients;

    for (int dir = 0; dir < direction_near_end; dir++) {
        const Direction direction = static_cast<Direction>(dir);
        Coefficients direction_coefficients;
        switch (m_diffusion_scheme) {
            case DiffusionSchemes::CentralDifferencing: {
                direction_coefficients = get_central_differencing_effects(direction);
                break;
            }
        }
        coefficients += direction_coefficients;
    }

    coefficients *= m_node->get_viscosity();

    return coefficients;
}

Coefficients Diffusion::get_central_differencing_effects(const Direction direction) const {
    Coefficients coefficients;

    Face *face = m_node->get_neighbouring_face(direction);
    double flux = m_node->get_dt();
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
        const double face_value = boundary_face->get_field_value(m_field);
        const double extra = 2.0 * flux;
        coefficients.center += extra;
        coefficients.source += face_value * extra;
    }

    return coefficients;
}
