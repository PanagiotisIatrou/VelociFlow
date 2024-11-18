#include "UnsteadySimulation.hpp"

#include <iostream>

UnsteadySimulation::UnsteadySimulation(Mesh *mesh, const double dt, const int timesteps,
                                       const double velocity_u_tolerance, const double velocity_v_tolerance,
                                       const double pressure_tolerance, const std::string output_file)
    : Simulation(mesh, velocity_u_tolerance, velocity_v_tolerance, pressure_tolerance, output_file) {
    m_dt = dt;
    m_timesteps = timesteps;
    m_mesh->set_dt(dt);

    // Propagate the dt to the nodes and the faces
    m_bulk_node_operations->set_dt(dt);
    m_bulk_face_operations->set_face_x_dt(dt);
    m_bulk_face_operations->set_face_y_dt(dt);
}

void UnsteadySimulation::solve() {
    m_timer->start_timer();

    calculate_active_cells_count();
    m_bulk_face_operations->update_face_x_velocities_distance_weighted();
    m_bulk_face_operations->update_face_y_velocities_distance_weighted();
    m_bulk_face_operations->update_face_x_viscosities();
    m_bulk_face_operations->update_face_y_viscosities();
    m_bulk_face_operations->update_face_x_pressures();
    m_bulk_face_operations->update_face_y_pressures();

    // TODO: Set custom initial conditions

    // Save the mesh settings
    m_saver->open_and_clear_file();
    m_saver->write_domain_size();
    m_saver->write_grid_size();
    m_saver->write_timesteps_count(m_timesteps);
    m_saver->write_dt();
    m_saver->write_execution_time(m_timer->get_elapsed_time());
    m_saver->close_file();

    for (int k = 0; k < m_timesteps; k++) {
        m_momentum_x_error = 1.0;
        m_momentum_y_error = 1.0;
        m_mass_imbalance = 1.0;
        m_mass_imbalance_residual_normalization_factor = 0.0;
        m_outer_iterations_count = 0;
        while (m_momentum_x_error > m_velocity_u_tolerance || m_momentum_y_error > m_velocity_v_tolerance ||
               m_mass_imbalance > m_pressure_tolerance) {
            simple_iterate(SimulationType::Unsteady);
            // std::cout << m_momentum_x_error << ", " << m_momentum_y_error << ", " << m_mass_imbalance << std::endl;
        }

        // Write the current timestep field values
        m_saver->open_append_file();
        m_saver->write_field(Field::VelocityU);
        m_saver->write_field(Field::VelocityV);
        m_saver->write_field(Field::Pressure);
        m_saver->close_file();

        // Save current field values as previous
        m_bulk_node_operations->update_node_previous_timestep_velocity_u();
        m_bulk_node_operations->update_node_previous_timestep_velocity_v();
        m_bulk_node_operations->update_node_previous_timestep_pressure();

        std::cout << "\r" << k + 1 << " / " << m_timesteps << std::flush;
    }

    m_time_taken = m_timer->get_elapsed_time();
}
