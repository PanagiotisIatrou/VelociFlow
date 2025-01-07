#include "SteadyConvectionDiffusionSimulation.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

SteadyConvectionDiffusionSimulation::SteadyConvectionDiffusionSimulation(
    Mesh* mesh, const double viscosity, const double tolerance_x, const double tolerance_y,
    std::string output_file, const VerboseType verbose_type)
    : Simulation(mesh, output_file, verbose_type) {
    m_viscosity = viscosity;
    m_tolerance_x = tolerance_x;
    m_tolerance_y = tolerance_y;

    // Create the equations
    m_equation_convection_diffusion_x = std::make_unique<ConvectionDiffusionX>(
        mesh, Field::VelocityX, 1.0, ResidualType::Scaled,
        StoppingRule::Relative, NormType::L1, 1e-3, false);
    m_equation_convection_diffusion_y = std::make_unique<ConvectionDiffusionY>(
        mesh, Field::VelocityY, 1.0, ResidualType::Scaled,
        StoppingRule::Relative, NormType::L1, 1e-3, false);

    // Populate all the nodes with the equation coefficients
    m_equation_convection_diffusion_x->populate_mesh();
    m_equation_convection_diffusion_y->populate_mesh();

    // Set viscosity
    m_bulk_node_operations->set_viscosity(m_viscosity);
    m_bulk_face_operations->set_face_x_viscosity(m_viscosity);
    m_bulk_face_operations->set_face_y_viscosity(m_viscosity);
}

void SteadyConvectionDiffusionSimulation::solve() {
    m_timer->start_timer();

    m_bulk_face_operations->update_face_x_velocities_distance_weighted();
    m_bulk_face_operations->update_face_y_velocities_distance_weighted();

    m_outer_iterations_count = 0;
    double first_convection_diffusion_x_error;
    double first_convection_diffusion_y_error;
    while (m_equation_convection_diffusion_x->get_imbalance() > m_tolerance_x ||
           m_equation_convection_diffusion_y->get_imbalance() > m_tolerance_y) {
        // Calculate the coefficients
        m_equation_convection_diffusion_x->calculate_coefficients();
        m_equation_convection_diffusion_y->calculate_coefficients();

        // Calculate the imbalance
        m_equation_convection_diffusion_x->calculate_imbalance();
        m_equation_convection_diffusion_y->calculate_imbalance();

        // Solve X and Y equations
        m_equation_convection_diffusion_x->solve();
        m_equation_convection_diffusion_y->solve();

        // Calculate the face velocities
        m_bulk_face_operations->update_face_x_velocities_distance_weighted();
        m_bulk_face_operations->update_face_y_velocities_distance_weighted();

        m_outer_iterations_count++;
        m_equation_convection_diffusion_x->progress_iteration_counter();
        m_equation_convection_diffusion_y->progress_iteration_counter();

        // First errors
        if (m_outer_iterations_count < 5) {
            first_convection_diffusion_x_error =
                m_equation_convection_diffusion_x->get_imbalance();
            first_convection_diffusion_y_error =
                m_equation_convection_diffusion_y->get_imbalance();
        }

        // Printing
        if (m_verbose_type == VerboseType::Residuals) {
            printf("%-6d   %4e   %4e\n", m_outer_iterations_count,
                   m_equation_convection_diffusion_x->get_imbalance(),
                   m_equation_convection_diffusion_y->get_imbalance());
        } else if (m_verbose_type == VerboseType::Percentages) {
            double convection_diffusion_x_scale;
            if (first_convection_diffusion_x_error <= m_tolerance_x) {
                convection_diffusion_x_scale =
                    (std::log10(m_tolerance_x) -
                     std::log10(
                         m_equation_convection_diffusion_x->get_imbalance() /
                         m_tolerance_x)) /
                    std::log10(m_tolerance_x);
            } else {
                convection_diffusion_x_scale =
                    std::log10(
                        first_convection_diffusion_x_error /
                        m_equation_convection_diffusion_x->get_imbalance()) /
                    std::log10(first_convection_diffusion_x_error /
                               m_tolerance_x);
            }
            convection_diffusion_x_scale =
                std::clamp(convection_diffusion_x_scale, 0.0, 1.0);
            const int convection_diffusion_x_percentage = static_cast<int>(
                std::floor(convection_diffusion_x_scale * 100.0));

            double convection_diffusion_y_scale;
            if (first_convection_diffusion_y_error <= m_tolerance_y) {
                convection_diffusion_y_scale =
                    (std::log10(m_tolerance_y) -
                     std::log10(
                         m_equation_convection_diffusion_y->get_imbalance() /
                         m_tolerance_y)) /
                    std::log10(m_tolerance_y);
            } else {
                convection_diffusion_y_scale =
                    std::log10(
                        first_convection_diffusion_y_error /
                        m_equation_convection_diffusion_y->get_imbalance()) /
                    std::log10(first_convection_diffusion_y_error /
                               m_tolerance_y);
            }
            convection_diffusion_y_scale =
                std::clamp(convection_diffusion_y_scale, 0.0, 1.0);
            const int convection_diffusion_y_percentage = static_cast<int>(
                std::floor(convection_diffusion_y_scale * 100.0));

            printf("\r[X: %-3d%%, Y: %-3d%%]",
                   convection_diffusion_x_percentage,
                   convection_diffusion_y_percentage);
            std::cout << std::flush;
        }
    }

    std::cout << std::endl
              << "Converged in " << m_outer_iterations_count << " iterations"
              << std::endl;

    m_time_taken = m_timer->get_elapsed_time();

    // Save the results
    m_saver->open_and_clear_file();
    m_saver->write_domain_size();
    m_saver->write_grid_size();
    m_saver->write_execution_time(m_time_taken);
    m_saver->write_field(Field::VelocityX);
    m_saver->write_field(Field::VelocityY);
    m_saver->close_file();
}
