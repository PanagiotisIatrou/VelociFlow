#pragma once

#include "../Mesh.hpp"

class BulkNodeOperations {
private:
    Mesh *m_mesh;
public:
    BulkNodeOperations(Mesh *mesh);

    // Velocity

    void correct_node_velocity_x() const;

    void correct_node_velocity_y() const;

    void update_node_previous_timestep_velocity_x() const;

    void update_node_previous_timestep_velocity_y() const;

    // Pressure

    void correct_node_pressure() const;

    void update_node_previous_timestep_pressure() const;

    // Dye

    void update_node_previous_timestep_dye() const;

    // Coefficients

    void calculate_momentum_coefficients(VelocityComponent velocity_component, SimulationType simulation_type) const;

    void calculate_pressure_coefficients() const;

    void calculate_dye_coefficients(SimulationType simulation_type) const;

    void calculate_convection_diffusion_coefficients(SimulationType simulation_type, VelocityComponent velocity_component) const;

    // Time

    void set_dt(double dt) const;
};
