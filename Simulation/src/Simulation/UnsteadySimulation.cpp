#include "UnsteadySimulation.hpp"

#include <iostream>
#include <cmath>
#include <algorithm>

UnsteadySimulation::UnsteadySimulation(Mesh *mesh, const double dt, const int timesteps,
                                       const double velocity_u_tolerance, const double velocity_v_tolerance,
                                       const double pressure_tolerance, const std::string output_file,
                                       const VerboseType verbose_type)
    : Simulation(mesh, velocity_u_tolerance, velocity_v_tolerance, pressure_tolerance, output_file, verbose_type) {
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
    m_bulk_face_operations->update_face_x_densities();
    m_bulk_face_operations->update_face_y_densities();
    m_bulk_face_operations->update_face_x_pressures();
    m_bulk_face_operations->update_face_y_pressures();

    // Save the mesh settings and the initial state
    m_saver->open_and_clear_file();
    m_saver->write_domain_size();
    m_saver->write_grid_size();
    m_saver->write_dt();
    m_saver->write_field(Field::VelocityU);
    m_saver->write_field(Field::VelocityV);
    m_saver->write_field(Field::Pressure);
    m_saver->close_file();

    m_mass_imbalance_residual_normalization_factor = 0.0;
    for (int k = 0; k < m_timesteps; k++) {
        m_momentum_x_error = 1.0;
        m_momentum_y_error = 1.0;
        m_mass_imbalance = 1.0;
        m_outer_iterations_count = 0;
        double first_momentum_x_error;
        double first_momentum_y_error;
        double first_mass_imbalance_error;
        while (m_momentum_x_error > m_velocity_u_tolerance || m_momentum_y_error > m_velocity_v_tolerance ||
               m_mass_imbalance > m_pressure_tolerance) {
            simple_iterate(SimulationType::Unsteady);

            // First errors
            if (m_outer_iterations_count < 5) {
                first_momentum_x_error = m_momentum_x_error;
                first_momentum_y_error = m_momentum_y_error;
                first_mass_imbalance_error = m_mass_imbalance;
            }

            // Printing
            if (m_verbose_type == VerboseType::Residuals) {
                printf("%.4e   %.4e   %.4e\n", m_momentum_x_error, m_momentum_y_error, m_mass_imbalance);
            } else if (m_verbose_type == VerboseType::Percentages) {
                double momentum_x_scale;
                if (first_momentum_x_error <= m_velocity_u_tolerance) {
                    momentum_x_scale = (std::log10(m_velocity_u_tolerance) - std::log10(m_momentum_x_error / m_velocity_u_tolerance)) / std::log10(m_velocity_u_tolerance);
                } else {
                    momentum_x_scale = std::log10(first_momentum_x_error / m_momentum_x_error) / std::log10(first_momentum_x_error / m_velocity_u_tolerance);
                }
                momentum_x_scale = std::clamp(momentum_x_scale, 0.0, 1.0);
                const int momentum_x_percentage = static_cast<int>(std::floor(momentum_x_scale * 100.0));

                double momentum_y_scale;
                if (first_momentum_y_error <= m_velocity_v_tolerance) {
                    momentum_y_scale = (std::log10(m_velocity_v_tolerance) - std::log10(m_momentum_y_error / m_velocity_v_tolerance)) / std::log10(m_velocity_v_tolerance);
                } else {
                    momentum_y_scale = std::log10(first_momentum_y_error / m_momentum_y_error) / std::log10(first_momentum_y_error / m_velocity_v_tolerance);
                }
                momentum_y_scale = std::clamp(momentum_y_scale, 0.0, 1.0);
                const int momentum_y_percentage = static_cast<int>(std::floor(momentum_y_scale * 100.0));

                double mass_imbalance_scale;
                if (first_mass_imbalance_error <= m_pressure_tolerance) {
                    mass_imbalance_scale = (std::log10(m_pressure_tolerance) - std::log10(m_momentum_y_error / m_pressure_tolerance)) / std::log10(m_pressure_tolerance);
                } else {
                    mass_imbalance_scale = std::log10(first_mass_imbalance_error / m_momentum_y_error) / std::log10(first_mass_imbalance_error / m_pressure_tolerance);
                }
                mass_imbalance_scale = std::clamp(mass_imbalance_scale, 0.0, 1.0);
                const int mass_imbalance_percentage = static_cast<int>(std::floor(mass_imbalance_scale * 100.0));

                const int timesteps_digits = m_timesteps > 0 ? static_cast<int>(log10(m_timesteps)) + 1 : 1;
                printf("\r[Timesteps: %-*.d / %-*.d Momentum X: %-3.d%%, Momentum Y: %-3.d%%, Continuity: %-3.d%%]", timesteps_digits, k + 1, timesteps_digits, m_timesteps, momentum_x_percentage, momentum_y_percentage, mass_imbalance_percentage);
                std::cout << std::flush;
            }
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
    }

    m_time_taken = m_timer->get_elapsed_time();

    // Save the time took to calculate
    m_saver->open_append_file();
    m_saver->write_execution_time(m_time_taken);
    m_saver->close_file();

    std::cout << std::endl;
}
