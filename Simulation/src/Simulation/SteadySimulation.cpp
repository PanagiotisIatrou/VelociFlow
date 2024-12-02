#include "SteadySimulation.hpp"

#include <cmath>
#include <iostream>

SteadySimulation::SteadySimulation(Mesh *mesh, const double velocity_u_tolerance, const double velocity_v_tolerance,
                                   const double pressure_tolerance, const std::string output_file,
                                   const VerboseType verbose_type)
    : Simulation(mesh, velocity_u_tolerance, velocity_v_tolerance, pressure_tolerance, output_file, verbose_type) {
}

void SteadySimulation::solve() {
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

    m_momentum_x_error = 1.0;
    m_momentum_y_error = 1.0;
    m_mass_imbalance = 1.0;
    m_outer_iterations_count = 0;
    double first_momentum_x_error;
    double first_momentum_y_error;
    double first_mass_imbalance_error;
    while (m_momentum_x_error > m_velocity_u_tolerance || m_momentum_y_error > m_velocity_v_tolerance ||
           m_mass_imbalance > m_pressure_tolerance) {
        simple_iterate(SimulationType::Steady);

        // First errors
        if (m_outer_iterations_count < 5) {
            first_momentum_x_error = m_momentum_x_error;
            first_momentum_y_error = m_momentum_y_error;
            first_mass_imbalance_error = m_mass_imbalance;
        }

        // Printing
        if (m_verbose_type == VerboseType::Residuals) {
            printf("%-6.d   %.4e   %.4e   %.4e\n", m_outer_iterations_count, m_momentum_x_error, m_momentum_y_error,
                   m_mass_imbalance);
        } else if (m_verbose_type == VerboseType::Percentages) {
            const double momentum_x_scale = 1 - std::log10(m_momentum_x_error / m_velocity_u_tolerance) / std::log10(first_momentum_x_error / m_velocity_u_tolerance);
            const int momentum_x_percentage = static_cast<int>(std::floor(momentum_x_scale * 100.0));
            const double momentum_y_scale = 1 - std::log10(m_momentum_y_error / m_velocity_v_tolerance) / std::log10(first_momentum_y_error / m_velocity_v_tolerance);
            const int momentum_y_percentage = static_cast<int>(std::floor(momentum_y_scale * 100.0));
            const double mass_imbalance_scale = 1 - std::log10(m_mass_imbalance / m_pressure_tolerance) / std::log10(first_mass_imbalance_error / m_pressure_tolerance);
            const int mass_imbalance_percentage = static_cast<int>(std::floor(mass_imbalance_scale * 100.0));

            printf("\r[Momentum X: %-3.d%%, Momentum Y: %-3.d%%, Continuity: %-3.d%%]", momentum_x_percentage, momentum_y_percentage, mass_imbalance_percentage);
            std::cout << std::flush;
        }
    }
    std::cout << std::endl << "Converged in " << m_outer_iterations_count << " iterations" << std::endl;

    m_time_taken = m_timer->get_elapsed_time();

    // Save the results
    m_saver->open_and_clear_file();

    m_saver->write_domain_size();
    m_saver->write_grid_size();
    m_saver->write_execution_time(m_time_taken);

    m_saver->write_field(Field::VelocityU);
    m_saver->write_field(Field::VelocityV);
    m_saver->write_field(Field::Pressure);

    m_saver->close_file();
}
