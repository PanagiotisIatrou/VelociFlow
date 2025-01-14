#pragma once

#include "../Mesh.hpp"

class BulkNodeOperations {
private:
    Mesh *m_mesh;

public:
    BulkNodeOperations(Mesh *mesh);

    void update_node_previous_timestep_fields() const;

    // Velocity

    void correct_node_velocity_x() const;

    void correct_node_velocity_y() const;

    // Pressure

    void correct_node_pressure() const;

    // Pressure correction

    void reset_pressure_correction() const;

    // Time

    void set_dt(double dt) const;

    // Density

    void set_density(double density) const;

    // Viscosity

    void set_viscosity(double viscosity) const;
};
