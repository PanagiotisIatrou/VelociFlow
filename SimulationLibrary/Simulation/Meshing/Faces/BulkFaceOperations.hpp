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

    // Face pressure corrections

    void update_face_x_pressure_corrections() const;

    void update_face_y_pressure_corrections() const;

    // Time

    void set_face_x_dt(double dt) const;

    void set_face_y_dt(double dt) const;

    // Density

    void set_face_x_density(double density) const;

    void set_face_y_density(double density) const;

    // Viscosity

    void set_face_x_viscosity(double viscosity) const;

    void set_face_y_viscosity(double viscosity) const;
};
