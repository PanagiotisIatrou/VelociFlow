#include "DiffusionSteady.hpp"

DiffusionSteady::DiffusionSteady(Mesh* mesh, const double viscosity, const double tolerance,
                                 const std::string output_file)
    : DiffusionSimulation(mesh, viscosity, tolerance, output_file, SimulationType::Steady) {
}

void DiffusionSteady::solve() {
    m_timer->start_timer();
    m_verbosity_handler->set_timesteps_count(1);

    // Calculate the coefficients
    m_equation_diffusion->calculate_coefficients();

    // Calculate the imbalance
    m_equation_diffusion->calculate_imbalance();

    // Solve the Diffusion equation
    m_equation_diffusion->solve();

    m_time_taken = m_timer->get_elapsed_time();

    // Save the results
    m_saver->open_and_clear_file();
    m_saver->write_simulation_name(SimulationName::DiffusionSteady);
    m_saver->write_domain_size(m_mesh->get_domain_size_x(), m_mesh->get_domain_size_y());
    m_saver->write_grid_size(m_mesh->get_size_x(), m_mesh->get_size_y());
    m_saver->write_viscosity(m_viscosity);
    m_saver->write_execution_time(m_time_taken);

    m_saver->write_field(Field::VelocityX);
    m_saver->close_file();
}
