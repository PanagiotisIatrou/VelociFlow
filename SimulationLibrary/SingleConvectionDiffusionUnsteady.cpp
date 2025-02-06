#include "SingleConvectionDiffusionUnsteady.hpp"

SingleConvectionDiffusionUnsteady::SingleConvectionDiffusionUnsteady(Mesh *mesh, const double viscosity,
                                                                     const double dt, const int timesteps,
                                                                     const double tolerance,
                                                                     const std::string output_file)
    : SingleConvectionDiffusionSimulation(mesh, viscosity, tolerance, output_file, SimulationType::Unsteady) {
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

void SingleConvectionDiffusionUnsteady::solve() {
    start_ncurses();

    m_timer->start_timer();

    // Save the mesh settings and the initial state
    m_saver->open_and_clear_file();
    m_saver->write_simulation_name(SimulationName::SingleConvectionDiffusionUnsteady);
    m_saver->write_domain_size(m_mesh->get_domain_size_x(), m_mesh->get_domain_size_y());
    m_saver->write_grid_size(m_mesh->get_size_x(), m_mesh->get_size_y());
    m_saver->write_viscosity(m_viscosity);
    m_saver->write_dt(m_dt);
    m_saver->write_field(Field::Phi);
    m_saver->close_file();

    bool has_quit = false;
    for (m_reached_timesteps = 0; m_reached_timesteps < m_timesteps; m_reached_timesteps++) {
        m_verbosity_handler->set_timesteps_count(m_reached_timesteps + 1);

        // Calculate the coefficients
        m_equation_single_convection_diffusion->calculate_coefficients();

        // Calculate the imbalance
        m_equation_single_convection_diffusion->calculate_imbalance();

        // Solve the Convection-Diffusion equation
        m_equation_single_convection_diffusion->solve();

        // Save the normalization values
        // TODO: Might fail if the first timestep converges too quickly (very rare)
        if (m_reached_timesteps == 0) {
            m_saver->open_append_file();
            m_saver->write_normalization_values(EquationType::SingleConvectionDiffusion,
                                                m_equation_single_convection_diffusion.get());
            m_saver->close_file();
        }

        m_verbosity_handler->print();

        if (pressed_quit()) {
            has_quit = true;
            break;
        }

        // Write the current timestep field values
        m_saver->open_append_file();
        m_saver->write_field(Field::Phi);
        m_saver->close_file();

        // Save current field values as previous
        m_bulk_node_operations->update_node_previous_timestep_fields();

        // Update the face values
        m_bulk_face_operations->update_face_x_phi();
        m_bulk_face_operations->update_face_y_phi();

        // Reset the residuals
        m_equation_single_convection_diffusion->reset_imbalance();
    }

    end_ncurses();

    if (has_quit) {
        std::cout << "Simulation stopped by user" << std::endl;
    }

    m_time_taken = m_timer->get_elapsed_time();

    // Save the time took to calculate
    m_saver->open_append_file();
    m_saver->write_execution_time(m_time_taken);
    m_saver->close_file();
}

int SingleConvectionDiffusionUnsteady::get_reached_timesteps() const {
    return m_reached_timesteps + 1;
}
