#include "NavierStokesSteady.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

NavierStokesSteady::NavierStokesSteady(Mesh* mesh, const double density, const double viscosity,
                                   const double tolerance_momentum_x, const double tolerance_momentum_y,
                                   const double tolerance_mass_imbalance, const std::string output_file,
                                   const VerbosityType verbosity_type)
    : NavierStokesSimulation(mesh, density, viscosity, tolerance_momentum_x, tolerance_momentum_y, tolerance_mass_imbalance,
                   output_file, SimulationType::Steady, verbosity_type) {
    // Verbosity
    m_verbosity_handler->enable_print_iterations();
}

void NavierStokesSteady::solve() {
    start_ncurses();

    m_timer->start_timer();
    m_verbosity_handler->set_timesteps_count(1);
    m_outer_iterations_count = 0;
    bool has_quit = false;
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

    // Solve the dye equation
    m_equation_dye->calculate_coefficients();
    m_equation_dye->solve();

    end_ncurses();

    if (has_quit) {
        std::cout << "Simulation stopped by user" << std::endl;
    }

    m_time_taken = m_timer->get_elapsed_time();

    // Save the results
    m_saver->open_and_clear_file();
    m_saver->write_simulation_name(SimulationName::NavierStokesSteady);
    m_saver->write_domain_size(m_mesh->get_domain_size_x(), m_mesh->get_domain_size_y());
    m_saver->write_grid_size(m_mesh->get_size_x(), m_mesh->get_size_y());
    m_saver->write_tolerance(EquationType::MomentumX, m_tolerance_momentum_x);
    m_saver->write_tolerance(EquationType::MomentumY, m_tolerance_momentum_y);
    m_saver->write_tolerance(EquationType::PressureCorrection, m_tolerance_mass_imbalance);
    m_saver->write_density(m_density);
    m_saver->write_viscosity(m_viscosity);
    m_saver->write_execution_time(m_time_taken);

    m_saver->write_field(Field::VelocityX);
    m_saver->write_field(Field::VelocityY);
    m_saver->write_field(Field::Pressure);
    m_saver->write_field(Field::Dye);

    m_saver->close_file();
}
