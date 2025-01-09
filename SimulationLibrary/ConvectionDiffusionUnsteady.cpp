#include "ConvectionDiffusionUnsteady.hpp"

#include <algorithm>
#include <cmath>

ConvectionDiffusionUnsteady::ConvectionDiffusionUnsteady(Mesh* mesh, const double viscosity, const double dt,
                                                         const int timesteps, const double tolerance_x,
                                                         const double tolerance_y, const std::string output_file,
                                                         const VerbosityType verbosity_type)
    : ConvectionDiffusion(mesh, viscosity, tolerance_x, tolerance_y, output_file, SimulationType::Unsteady,
                          verbosity_type) {
    m_dt = dt;
    m_timesteps = timesteps;
    m_mesh->set_dt(dt);

    // Save current field values as previous
    m_bulk_node_operations->update_node_previous_timestep_fields();

    // Propagate the dt to the nodes and the faces
    m_bulk_node_operations->set_dt(dt);
    m_bulk_face_operations->set_face_x_dt(dt);
    m_bulk_face_operations->set_face_y_dt(dt);

    // Verbosity
    m_verbosity_handler->enable_print_timesteps(timesteps);
}

void ConvectionDiffusionUnsteady::solve() {
    m_timer->start_timer();

    // Save the mesh settings and the initial state
    m_saver->open_and_clear_file();
    m_saver->write_domain_size(m_mesh->get_domain_size_x(), m_mesh->get_domain_size_y());
    m_saver->write_grid_size(m_mesh->get_size_x(), m_mesh->get_size_y());
    m_saver->write_viscosity(m_viscosity);
    m_saver->write_dt(m_dt);
    m_saver->write_field(Field::VelocityX);
    m_saver->write_field(Field::VelocityY);
    m_saver->close_file();

    for (int k = 0; k < m_timesteps; k++) {
        m_outer_iterations_count = 0;
        m_verbosity_handler->set_timesteps_count(k + 1);
        while (m_equation_convection_diffusion_x->get_imbalance() > m_tolerance_x ||
               m_equation_convection_diffusion_y->get_imbalance() > m_tolerance_y) {
            iterate();

            m_verbosity_handler->set_iterations_count(m_outer_iterations_count);
            m_verbosity_handler->print();
        }

        // Write the current timestep field values
        m_saver->open_append_file();
        m_saver->write_field(Field::VelocityX);
        m_saver->write_field(Field::VelocityY);
        m_saver->close_file();

        // Save current field values as previous
        m_bulk_node_operations->update_node_previous_timestep_fields();

        // Reset the residuals
        m_equation_convection_diffusion_x->reset_imbalance();
        m_equation_convection_diffusion_y->reset_imbalance();
    }

    m_time_taken = m_timer->get_elapsed_time();

    // Save the time took to calculate
    m_saver->open_append_file();
    m_saver->write_execution_time(m_time_taken);
    m_saver->close_file();

    std::cout << std::endl;
}
