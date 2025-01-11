#include "NavierStokesUnsteady.hpp"

#include <iostream>

NavierStokesUnsteady::NavierStokesUnsteady(Mesh *mesh, const double density, const double viscosity, const double dt,
                                       const int timesteps, const double tolerance_momentum_x,
                                       const double tolerance_momentum_y, const double tolerance_mass_imbalance,
                                       const std::string output_file, const VerbosityType verbosity_type)
    : NavierStokesSimulation(mesh, density, viscosity, tolerance_momentum_x, tolerance_momentum_y, tolerance_mass_imbalance,
                   output_file, SimulationType::Unsteady, verbosity_type) {
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

void NavierStokesUnsteady::solve() {
    start_ncurses();

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

    bool has_quit = false;
    for (int k = 0; k < m_timesteps; k++) {
        m_outer_iterations_count = 0;
        m_verbosity_handler->set_timesteps_count(k + 1);
        while (m_equation_momentum_x->get_imbalance() > m_tolerance_momentum_x ||
               m_equation_momentum_y->get_imbalance() > m_tolerance_momentum_y ||
               m_equation_pressure_correction->get_mass_imbalance() > m_tolerance_mass_imbalance) {
            simple_iterate();

            m_verbosity_handler->set_iterations_count(m_outer_iterations_count);
            m_verbosity_handler->print();

            if (pressed_quit()) {
                has_quit = true;
                break;
            }
        }

        if (has_quit) {
            break;
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
