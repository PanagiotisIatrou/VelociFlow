#pragma once
#include "Meshing/Mesh.hpp"

class BulkNodeOperations {
private:
    Mesh *m_mesh;
public:
    BulkNodeOperations(Mesh *mesh);

    // Velocity

    void correct_node_velocity_u() const;

    void correct_node_velocity_v() const;

    // Pressure

    void correct_node_pressure() const;

    // Coefficients

    void calculate_momentum_coefficients(VelocityComponent velocity_component) const;

    void calculate_pressure_coefficients() const;
};
