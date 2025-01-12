#include "ConvectionDiffusionSteady.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

ConvectionDiffusionSteady::ConvectionDiffusionSteady(Mesh* mesh, const double viscosity,
                                                                         const double tolerance_x,
                                                                         const double tolerance_y,
                                                                         const std::string output_file,
                                                                         const VerbosityType verbosity_type)
    : ConvectionDiffusionSimulation(mesh, viscosity, tolerance_x, tolerance_y, output_file, SimulationType::Steady,
                          verbosity_type) {
    // Verbosity
    m_verbosity_handler->enable_print_iterations();
}

void ConvectionDiffusionSteady::solve() {
    start_ncurses();

    m_timer->start_timer();
    m_verbosity_handler->set_timesteps_count(1);
    m_outer_iterations_count = 0;
    bool has_quit = false;
    while (m_equation_convection_diffusion_x->get_imbalance() > m_tolerance_x ||
           m_equation_convection_diffusion_y->get_imbalance() > m_tolerance_y) {
        iterate();

        m_verbosity_handler->set_iterations_count(m_outer_iterations_count);
        m_verbosity_handler->print();

        if (pressed_quit()) {
            has_quit = true;
            break;
        }
    }

    end_ncurses();

    if (has_quit) {
        std::cout << "Simulation stopped by user" << std::endl;
    }

    m_time_taken = m_timer->get_elapsed_time();

    // Save the results
    m_saver->open_and_clear_file();
    m_saver->write_simulation_name(SimulationName::ConvectionDiffusionSteady);
    m_saver->write_domain_size(m_mesh->get_domain_size_x(), m_mesh->get_domain_size_y());
    m_saver->write_grid_size(m_mesh->get_size_x(), m_mesh->get_size_y());
    m_saver->write_viscosity(m_viscosity);
    m_saver->write_execution_time(m_time_taken);

    m_saver->write_field(Field::VelocityX);
    m_saver->write_field(Field::VelocityY);
    m_saver->close_file();
}
