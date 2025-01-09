#include "NavierStokesUnsteady.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

NavierStokesUnsteady::NavierStokesUnsteady(Mesh *mesh, const double density, const double viscosity, const double dt,
                                       const int timesteps, const double tolerance_momentum_x,
                                       const double tolerance_momentum_y, const double tolerance_mass_imbalance,
                                       const std::string output_file, const VerboseType verbose_type)
    : NavierStokes(mesh, density, viscosity, tolerance_momentum_x, tolerance_momentum_y, tolerance_mass_imbalance,
                   output_file, SimulationType::Unsteady, verbose_type) {
    m_dt = dt;
    m_timesteps = timesteps;
    m_mesh->set_dt(dt);

    // Save current field values as previous
    m_bulk_node_operations->update_node_previous_timestep_fields();

    // Propagate the dt to the nodes and the faces
    m_bulk_node_operations->set_dt(dt);
    m_bulk_face_operations->set_face_x_dt(dt);
    m_bulk_face_operations->set_face_y_dt(dt);
}

void NavierStokesUnsteady::solve() {
    m_timer->start_timer();

    // Save the mesh settings and the initial state
    m_saver->open_and_clear_file();
    m_saver->write_domain_size(m_mesh->get_domain_size_x(), m_mesh->get_domain_size_y());
    m_saver->write_grid_size(m_mesh->get_size_x(), m_mesh->get_size_y());
    m_saver->write_density(m_density);
    m_saver->write_viscosity(m_viscosity);
    m_saver->write_dt(m_dt);
    m_saver->write_field(Field::VelocityX);
    m_saver->write_field(Field::VelocityY);
    m_saver->write_field(Field::Pressure);
    m_saver->write_field(Field::Dye);
    m_saver->close_file();

    for (int k = 0; k < m_timesteps; k++) {
        m_outer_iterations_count = 0;
        double first_momentum_x_error;
        double first_momentum_y_error;
        double first_mass_imbalance_error;
        while (m_equation_momentum_x->get_imbalance() > m_tolerance_momentum_x ||
               m_equation_momentum_y->get_imbalance() > m_tolerance_momentum_y ||
               m_equation_pressure_correction->get_mass_imbalance() > m_tolerance_mass_imbalance) {
            simple_iterate();

            // First errors
            if (m_outer_iterations_count < 5) {
                first_momentum_x_error = m_equation_momentum_x->get_imbalance();
                first_momentum_y_error = m_equation_momentum_y->get_imbalance();
                first_mass_imbalance_error = m_equation_pressure_correction->get_mass_imbalance();
            }

            // Printing
            if (m_verbose_type == VerboseType::Residuals) {
                printf("%-6d   %4e   %4e   %4e\n", m_outer_iterations_count, m_equation_momentum_x->get_imbalance(),
                       m_equation_momentum_y->get_imbalance(), m_equation_pressure_correction->get_mass_imbalance());
            } else if (m_verbose_type == VerboseType::Percentages) {
                double momentum_x_scale;
                if (first_momentum_x_error <= m_tolerance_momentum_x) {
                    momentum_x_scale = (std::log10(m_tolerance_momentum_x) -
                                        std::log10(m_equation_momentum_x->get_imbalance() / m_tolerance_momentum_x)) /
                                       std::log10(m_tolerance_momentum_x);
                } else {
                    momentum_x_scale = std::log10(first_momentum_x_error / m_equation_momentum_x->get_imbalance()) /
                                       std::log10(first_momentum_x_error / m_tolerance_momentum_x);
                }
                momentum_x_scale = std::clamp(momentum_x_scale, 0.0, 1.0);
                const int momentum_x_percentage = static_cast<int>(std::floor(momentum_x_scale * 100.0));

                double momentum_y_scale;
                if (first_momentum_y_error <= m_tolerance_momentum_y) {
                    momentum_y_scale = (std::log10(m_tolerance_momentum_y) -
                                        std::log10(m_equation_momentum_y->get_imbalance() / m_tolerance_momentum_y)) /
                                       std::log10(m_tolerance_momentum_y);
                } else {
                    momentum_y_scale = std::log10(first_momentum_y_error / m_equation_momentum_y->get_imbalance()) /
                                       std::log10(first_momentum_y_error / m_tolerance_momentum_y);
                }
                momentum_y_scale = std::clamp(momentum_y_scale, 0.0, 1.0);
                const int momentum_y_percentage = static_cast<int>(std::floor(momentum_y_scale * 100.0));

                double mass_imbalance_scale;
                if (first_mass_imbalance_error <= m_tolerance_mass_imbalance) {
                    mass_imbalance_scale = (std::log10(m_tolerance_mass_imbalance) -
                                            std::log10(m_equation_pressure_correction->get_mass_imbalance() /
                                                       m_tolerance_mass_imbalance)) /
                                           std::log10(m_tolerance_mass_imbalance);
                } else {
                    mass_imbalance_scale =
                        std::log10(first_mass_imbalance_error / m_equation_pressure_correction->get_mass_imbalance()) /
                        std::log10(first_mass_imbalance_error / m_tolerance_mass_imbalance);
                }
                mass_imbalance_scale = std::clamp(mass_imbalance_scale, 0.0, 1.0);
                const int mass_imbalance_percentage = static_cast<int>(std::floor(mass_imbalance_scale * 100.0));

                printf("\rTimesteps: %6d / %d [Momentum X: %-3d%%, Momentum Y: %-3d%%, Continuity: %-3d%%]", k + 1,
                       m_timesteps, momentum_x_percentage, momentum_y_percentage, mass_imbalance_percentage);
                std::cout << std::flush;
            }
        }

        // Solve the dye equation
        m_equation_dye->calculate_coefficients();
        m_equation_dye->solve();

        m_bulk_face_operations->update_face_x_dye();
        m_bulk_face_operations->update_face_y_dye();

        // Write the current timestep field values
        m_saver->open_append_file();
        m_saver->write_field(Field::VelocityX);
        m_saver->write_field(Field::VelocityY);
        m_saver->write_field(Field::Pressure);
        m_saver->write_field(Field::Dye);
        m_saver->close_file();

        // Save current field values as previous
        m_bulk_node_operations->update_node_previous_timestep_fields();

        // Reset the residuals
        m_equation_momentum_x->reset_imbalance();
        m_equation_momentum_y->reset_imbalance();
        m_equation_pressure_correction->reset_imbalance();
        m_equation_pressure_correction->reset_mass_imbalance();
        m_equation_dye->reset_imbalance();
    }

    m_time_taken = m_timer->get_elapsed_time();

    // Save the time took to calculate
    m_saver->open_append_file();
    m_saver->write_execution_time(m_time_taken);
    m_saver->close_file();

    std::cout << std::endl;
}
