#include "BulkFaceOperations.hpp"

#include "Interior/InteriorFace.hpp"

BulkFaceOperations::BulkFaceOperations(Mesh *mesh) {
    m_mesh = mesh;
}

// Face velocity

void BulkFaceOperations::correct_face_x_velocity() const {
    for (int i = 0; i < m_mesh->get_size_x() + 1; i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Face *face = m_mesh->get_face_x(i, j);

            // Nothing to calculate for a boundary face
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->correct_velocity();
        }
    }
}

void BulkFaceOperations::correct_face_y_velocity() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y() + 1; j++) {
            Face *face = m_mesh->get_face_y(i, j);

            // Nothing to calculate for a boundary face
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->correct_velocity();
        }
    }
}

void BulkFaceOperations::update_face_x_velocities_distance_weighted() const {
    for (int i = 0; i < m_mesh->get_size_x() + 1; i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Face *face = m_mesh->get_face_x(i, j);

            // Nothing to calculate for boundary faces
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_velocity_distance_weighted();
        }
    }
}

void BulkFaceOperations::update_face_y_velocities_distance_weighted() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y() + 1; j++) {
            Face *face = m_mesh->get_face_y(i, j);

            // Nothing to calculate for boundary faces
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_velocity_distance_weighted();
        }
    }
}

void BulkFaceOperations::update_face_x_velocities_rhie_chow() const {
    for (int i = 0; i < m_mesh->get_size_x() + 1; i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Face *face = m_mesh->get_face_x(i, j);

            // Nothing to calculate for boundary faces
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_velocity_rhie_chow();
        }
    }
}

void BulkFaceOperations::update_face_y_velocities_rhie_chow() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y() + 1; j++) {
            Face *face = m_mesh->get_face_y(i, j);

            // Nothing to calculate for boundary faces
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_velocity_rhie_chow();
        }
    }
}

// Face viscosity

void BulkFaceOperations::update_face_x_viscosities() const {
    for (int i = 0; i < m_mesh->get_size_x() + 1; i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Face *face = m_mesh->get_face_x(i, j);

            // Nothing to calculate on boundary faces
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_viscosity();
        }
    }
}

void BulkFaceOperations::update_face_y_viscosities() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y() + 1; j++) {
            Face *face = m_mesh->get_face_y(i, j);

            // Nothing to calculate on boundary faces
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_viscosity();
        }
    }
}

// Face density

void BulkFaceOperations::update_face_x_densities() const {
    for (int i = 0; i < m_mesh->get_size_x() + 1; i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Face *face = m_mesh->get_face_x(i, j);

            // Nothing to calculate on boundary faces
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_density();
        }
    }
}

void BulkFaceOperations::update_face_y_densities() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y() + 1; j++) {
            Face *face = m_mesh->get_face_y(i, j);

            // Nothing to calculate on boundary faces
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_density();
        }
    }
}

// Face pressure

void BulkFaceOperations::update_face_x_pressures() const {
    for (int i = 0; i < m_mesh->get_size_x() + 1; i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Face *face = m_mesh->get_face_x(i, j);

            // Nothing to calculate on boundary faces
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_pressure();
        }
    }
}

void BulkFaceOperations::update_face_y_pressures() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y() + 1; j++) {
            Face *face = m_mesh->get_face_y(i, j);

            // Nothing to calculate on boundary faces
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_pressure();
        }
    }
}

// Face Dye

void BulkFaceOperations::update_face_x_dye() const {
    for (int i = 0; i < m_mesh->get_size_x() + 1; i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Face *face = m_mesh->get_face_x(i, j);

            // Nothing to calculate on boundary faces
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_dye();
        }
    }
}

void BulkFaceOperations::update_face_y_dye() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y() + 1; j++) {
            Face *face = m_mesh->get_face_y(i, j);

            // Nothing to calculate on boundary faces
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_dye();
        }
    }
}

// Face pressure corrections

void BulkFaceOperations::update_face_x_pressure_corrections() const {
    for (int i = 0; i < m_mesh->get_size_x() + 1; i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Face *face = m_mesh->get_face_x(i, j);

            // Nothing to calculate on boundary faces
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_pressure_correction();
        }
    }
}

void BulkFaceOperations::update_face_y_pressure_corrections() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y() + 1; j++) {
            Face *face = m_mesh->get_face_y(i, j);

            // Nothing to calculate on boundary faces
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_pressure_correction();
        }
    }
}

// Time

void BulkFaceOperations::set_face_x_dt(const double dt) const {
    for (int i = 0; i < m_mesh->get_size_x() + 1; i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Face *face = m_mesh->get_face_x(i, j);

            // Nothing to calculate on boundary faces
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->set_dt(dt);
        }
    }
}

void BulkFaceOperations::set_face_y_dt(const double dt) const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y() + 1; j++) {
            Face *face = m_mesh->get_face_y(i, j);

            // Nothing to calculate on boundary faces
            if (face == nullptr || face->get_face_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->set_dt(dt);
        }
    }
}
