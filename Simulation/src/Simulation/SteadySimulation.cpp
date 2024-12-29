#include "SteadySimulation.hpp"

#include <cmath>
#include <iostream>
#include <algorithm>

SteadySimulation::SteadySimulation(Mesh *mesh, const double tolerance_velocity_x, const double tolerance_velocity_y,
                                   const double pressure_tolerance, const std::string output_file,
                                   const VerboseType verbose_type)
    : Simulation(mesh, tolerance_velocity_x, tolerance_velocity_y, pressure_tolerance, output_file, verbose_type) {
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
    m_bulk_face_operations->update_face_x_dye();
    m_bulk_face_operations->update_face_y_dye();

    m_momentum_x_error = 1.0;
    m_momentum_y_error = 1.0;
    m_mass_imbalance = 1.0;
    m_outer_iterations_count = 0;
    double first_momentum_x_error;
    double first_momentum_y_error;
    double first_mass_imbalance_error;
    while (m_momentum_x_error > m_tolerance_velocity_x || m_momentum_y_error > m_tolerance_velocity_y ||
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
            double momentum_x_scale;
            if (first_momentum_x_error <= m_tolerance_velocity_x) {
                momentum_x_scale = (std::log10(m_tolerance_velocity_x) - std::log10(m_momentum_x_error / m_tolerance_velocity_x)) / std::log10(m_tolerance_velocity_x);
            } else {
                momentum_x_scale = std::log10(first_momentum_x_error / m_momentum_x_error) / std::log10(first_momentum_x_error / m_tolerance_velocity_x);
            }
            momentum_x_scale = std::clamp(momentum_x_scale, 0.0, 1.0);
            const int momentum_x_percentage = static_cast<int>(std::floor(momentum_x_scale * 100.0));

            double momentum_y_scale;
            if (first_momentum_y_error <= m_tolerance_velocity_y) {
                momentum_y_scale = (std::log10(m_tolerance_velocity_y) - std::log10(m_momentum_y_error / m_tolerance_velocity_y)) / std::log10(m_tolerance_velocity_y);
            } else {
                momentum_y_scale = std::log10(first_momentum_y_error / m_momentum_y_error) / std::log10(first_momentum_y_error / m_tolerance_velocity_y);
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

            printf("\r[Momentum X: %-3.d%%, Momentum Y: %-3.d%%, Continuity: %-3.d%%]", momentum_x_percentage, momentum_y_percentage, mass_imbalance_percentage);
            std::cout << std::flush;
        }
    }

    // Solve the dye equation
    m_bulk_node_operations->calculate_dye_coefficients(SimulationType::Steady);
    solve_dye();

    std::cout << std::endl << "Converged in " << m_outer_iterations_count << " iterations" << std::endl;

    m_time_taken = m_timer->get_elapsed_time();

    // Save the results
    m_saver->open_and_clear_file();

    m_saver->write_domain_size();
    m_saver->write_grid_size();
    m_saver->write_execution_time(m_time_taken);

    m_saver->write_field(Field::VelocityX);
    m_saver->write_field(Field::VelocityY);
    m_saver->write_field(Field::Pressure);
    m_saver->write_field(Field::Dye);

    m_saver->close_file();
}
