#include "SteadyConvectionDiffusionSimulation.hpp"

#include <iostream>
#include <cmath>
#include <algorithm>

SteadyConvectionDiffusionSimulation::SteadyConvectionDiffusionSimulation(Mesh *mesh, double velocity_u_tolerance, double velocity_v_tolerance,
                                                                         std::string output_file, VerboseType verbose_type) : Simulation(mesh, velocity_u_tolerance, velocity_v_tolerance, 0.0, output_file, verbose_type) {

}

void SteadyConvectionDiffusionSimulation::solve() {
    m_timer->start_timer();

    calculate_active_cells_count();
    m_bulk_face_operations->update_face_x_velocities_distance_weighted();
    m_bulk_face_operations->update_face_y_velocities_distance_weighted();
    m_bulk_face_operations->update_face_x_viscosities();
    m_bulk_face_operations->update_face_y_viscosities();
    m_bulk_face_operations->update_face_x_densities();
    m_bulk_face_operations->update_face_y_densities();

    m_convection_diffusion_x_error = 1.0;
    m_convection_diffusion_y_error = 1.0;
    m_outer_iterations_count = 0;
    double first_convection_diffusion_x_error;
    double first_convection_diffusion_y_error;
    while (m_convection_diffusion_x_error > m_velocity_u_tolerance || m_convection_diffusion_y_error > m_velocity_v_tolerance) {
        convection_diffusion_iterate(SimulationType::Steady);

        // First errors
        if (m_outer_iterations_count < 5) {
            first_convection_diffusion_x_error = m_convection_diffusion_x_error;
            first_convection_diffusion_y_error = m_convection_diffusion_y_error;
        }

        // Printing
        if (m_verbose_type == VerboseType::Residuals) {
            printf("%-6.d   %.4e   %.4e\n", m_outer_iterations_count, m_convection_diffusion_x_error, m_convection_diffusion_y_error);
        } else if (m_verbose_type == VerboseType::Percentages) {
            double convection_diffusion_x_scale;
            if (first_convection_diffusion_x_error <= m_velocity_u_tolerance) {
                convection_diffusion_x_scale = (std::log10(m_velocity_u_tolerance) - std::log10(m_convection_diffusion_x_error / m_velocity_u_tolerance)) / std::log10(m_velocity_u_tolerance);
            } else {
                convection_diffusion_x_scale = std::log10(first_convection_diffusion_x_error / m_convection_diffusion_x_error) / std::log10(first_convection_diffusion_x_error / m_velocity_u_tolerance);
            }
            convection_diffusion_x_scale = std::clamp(convection_diffusion_x_scale, 0.0, 1.0);
            const int convection_diffusion_x_percentage = static_cast<int>(std::floor(convection_diffusion_x_scale * 100.0));

            double convection_diffusion_y_scale;
            if (first_convection_diffusion_y_error <= m_velocity_v_tolerance) {
                convection_diffusion_y_scale = (std::log10(m_velocity_v_tolerance) - std::log10(m_convection_diffusion_y_error / m_velocity_v_tolerance)) / std::log10(m_velocity_v_tolerance);
            } else {
                convection_diffusion_y_scale = std::log10(first_convection_diffusion_y_error / m_convection_diffusion_y_error) / std::log10(first_convection_diffusion_y_error / m_velocity_v_tolerance);
            }
            convection_diffusion_y_scale = std::clamp(convection_diffusion_y_scale, 0.0, 1.0);
            const int convection_diffusion_y_percentage = static_cast<int>(std::floor(convection_diffusion_y_scale * 100.0));

            printf("\r[X: %-3.d%%, Y: %-3.d%%]", convection_diffusion_x_percentage, convection_diffusion_y_percentage);
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
    m_saver->close_file();
}