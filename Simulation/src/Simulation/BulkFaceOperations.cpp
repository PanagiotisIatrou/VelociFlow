#include "BulkFaceOperations.h"

#include "Meshing/Faces/Interior/InteriorFace.hpp"

BulkFaceOperations::BulkFaceOperations(Mesh *mesh) {
    m_mesh = mesh;
}

// Face velocity

void BulkFaceOperations::correct_face_x_velocity() const {
    for (int i = 0; i < m_mesh->get_size_x() + 1; i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Face *face = m_mesh->get_face_x(i, j);

            // Nothing to calculate for a boundary face
            if (face == nullptr || face->get_type() == FaceType::Boundary) {
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
            if (face == nullptr || face->get_type() == FaceType::Boundary) {
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
            if (face == nullptr || face->get_type() == FaceType::Boundary) {
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
            if (face == nullptr || face->get_type() == FaceType::Boundary) {
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
            if (face == nullptr || face->get_type() == FaceType::Boundary) {
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
            if (face == nullptr || face->get_type() == FaceType::Boundary) {
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
            if (face == nullptr || face->get_type() == FaceType::Boundary) {
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
            if (face == nullptr || face->get_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_viscosity();
        }
    }
}

// Face pressure

void BulkFaceOperations::update_face_x_pressures() const {
    for (int i = 0; i < m_mesh->get_size_x() + 1; i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Face *face = m_mesh->get_face_x(i, j);

            // Nothing to calculate on boundary faces
            if (face == nullptr || face->get_type() == FaceType::Boundary) {
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
            if (face == nullptr || face->get_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_pressure();
        }
    }
}

// Face pressure corrections

void BulkFaceOperations::update_face_x_pressure_corrections() const {
    for (int i = 0; i < m_mesh->get_size_x() + 1; i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Face *face = m_mesh->get_face_x(i, j);

            // Nothing to calculate on boundary faces
            if (face == nullptr || face->get_type() == FaceType::Boundary) {
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
            if (face == nullptr || face->get_type() == FaceType::Boundary) {
                continue;
            }

            static_cast<InteriorFace *>(face)->update_pressure_correction();
        }
    }
}
