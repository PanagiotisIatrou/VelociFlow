#pragma once

#include "../Mesh.hpp"

class BulkFaceOperations {
private:
    Mesh *m_mesh;

public:
    BulkFaceOperations(Mesh *mesh);

    // Face velocities

    void update_face_x_velocities_distance_weighted() const;

    void update_face_y_velocities_distance_weighted() const;

    void update_face_x_velocities_rhie_chow() const;

    void update_face_y_velocities_rhie_chow() const;

    void correct_face_x_velocity() const;

    void correct_face_y_velocity() const;

    // Face pressures

    void update_face_x_pressures() const;

    void update_face_y_pressures() const;

    // Dye

    void update_face_x_dye() const;

    void update_face_y_dye() const;

    // Pressure corrections

    void update_face_x_pressure_corrections() const;

    void update_face_y_pressure_corrections() const;

    // Phi

    void update_face_x_phi() const;

    void update_face_y_phi() const;

    // Time

    void set_face_x_dt(double dt) const;

    void set_face_y_dt(double dt) const;
};
