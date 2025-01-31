#include "DiffusionSteady.hpp"

DiffusionSteady::DiffusionSteady(Mesh* mesh, const double viscosity, const double tolerance_x, const double tolerance_y,
                                 const std::string output_file)
    : DiffusionSimulation(mesh, viscosity, tolerance_x, tolerance_y, output_file, SimulationType::Steady) {
}

void DiffusionSteady::solve() {
    m_timer->start_timer();
    m_verbosity_handler->set_timesteps_count(1);
    m_outer_iterations_count = 0;

    iterate();

    m_time_taken = m_timer->get_elapsed_time();

    // Save the results
    m_saver->open_and_clear_file();
    m_saver->write_simulation_name(SimulationName::DiffusionSteady);
    m_saver->write_domain_size(m_mesh->get_domain_size_x(), m_mesh->get_domain_size_y());
    m_saver->write_grid_size(m_mesh->get_size_x(), m_mesh->get_size_y());
    m_saver->write_tolerance(EquationType::DiffusionX, m_tolerance_x);
    m_saver->write_tolerance(EquationType::DiffusionY, m_tolerance_y);
    m_saver->write_viscosity(m_viscosity);
    m_saver->write_execution_time(m_time_taken);

    m_saver->write_field(Field::VelocityX);
    m_saver->write_field(Field::VelocityY);
    m_saver->close_file();
}
