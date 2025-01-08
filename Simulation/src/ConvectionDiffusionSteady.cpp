#include "ConvectionDiffusionSteady.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

ConvectionDiffusionSteady::ConvectionDiffusionSteady(Mesh* mesh, const double viscosity,
                                                                         const double tolerance_x,
                                                                         const double tolerance_y,
                                                                         const std::string output_file,
                                                                         const VerboseType verbose_type)
    : ConvectionDiffusion(mesh, viscosity, tolerance_x, tolerance_y, output_file, SimulationType::Steady,
                          verbose_type) {
}

void ConvectionDiffusionSteady::solve() {
    m_timer->start_timer();

    m_outer_iterations_count = 0;
    double first_convection_diffusion_x_error;
    double first_convection_diffusion_y_error;
    while (m_equation_convection_diffusion_x->get_imbalance() > m_tolerance_x ||
           m_equation_convection_diffusion_y->get_imbalance() > m_tolerance_y) {
        iterate();

        // First errors
        if (m_outer_iterations_count < 5) {
            first_convection_diffusion_x_error = m_equation_convection_diffusion_x->get_imbalance();
            first_convection_diffusion_y_error = m_equation_convection_diffusion_y->get_imbalance();
        }

        // Printing
        if (m_verbose_type == VerboseType::Residuals) {
            printf("%-6d   %4e   %4e\n", m_outer_iterations_count, m_equation_convection_diffusion_x->get_imbalance(),
                   m_equation_convection_diffusion_y->get_imbalance());
        } else if (m_verbose_type == VerboseType::Percentages) {
            double convection_diffusion_x_scale;
            if (first_convection_diffusion_x_error <= m_tolerance_x) {
                convection_diffusion_x_scale =
                    (std::log10(m_tolerance_x) -
                     std::log10(m_equation_convection_diffusion_x->get_imbalance() / m_tolerance_x)) /
                    std::log10(m_tolerance_x);
            } else {
                convection_diffusion_x_scale = std::log10(first_convection_diffusion_x_error /
                                                          m_equation_convection_diffusion_x->get_imbalance()) /
                                               std::log10(first_convection_diffusion_x_error / m_tolerance_x);
            }
            convection_diffusion_x_scale = std::clamp(convection_diffusion_x_scale, 0.0, 1.0);
            const int convection_diffusion_x_percentage =
                static_cast<int>(std::floor(convection_diffusion_x_scale * 100.0));

            double convection_diffusion_y_scale;
            if (first_convection_diffusion_y_error <= m_tolerance_y) {
                convection_diffusion_y_scale =
                    (std::log10(m_tolerance_y) -
                     std::log10(m_equation_convection_diffusion_y->get_imbalance() / m_tolerance_y)) /
                    std::log10(m_tolerance_y);
            } else {
                convection_diffusion_y_scale = std::log10(first_convection_diffusion_y_error /
                                                          m_equation_convection_diffusion_y->get_imbalance()) /
                                               std::log10(first_convection_diffusion_y_error / m_tolerance_y);
            }
            convection_diffusion_y_scale = std::clamp(convection_diffusion_y_scale, 0.0, 1.0);
            const int convection_diffusion_y_percentage =
                static_cast<int>(std::floor(convection_diffusion_y_scale * 100.0));

            printf("\r[X: %-3d%%, Y: %-3d%%]", convection_diffusion_x_percentage, convection_diffusion_y_percentage);
            std::cout << std::flush;
        }
    }

    std::cout << std::endl << "Converged in " << m_outer_iterations_count << " iterations" << std::endl;

    m_time_taken = m_timer->get_elapsed_time();

    // Save the results
    m_saver->open_and_clear_file();
    m_saver->write_domain_size(m_mesh->get_domain_size_x(), m_mesh->get_domain_size_y());
    m_saver->write_grid_size(m_mesh->get_size_x(), m_mesh->get_size_y());
    m_saver->write_viscosity(m_viscosity);
    m_saver->write_execution_time(m_time_taken);

    m_saver->write_field(Field::VelocityX);
    m_saver->write_field(Field::VelocityY);
    m_saver->close_file();
}
