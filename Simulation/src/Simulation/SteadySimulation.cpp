#include "SteadySimulation.hpp"

#include <iostream>

SteadySimulation::SteadySimulation(Mesh *mesh, const double velocity_u_tolerance, const double velocity_v_tolerance,
                                   const double pressure_tolerance, const std::string output_file)
    : Simulation(mesh, velocity_u_tolerance, velocity_v_tolerance, pressure_tolerance, output_file) {
}

void SteadySimulation::solve() {
    m_timer->start_timer();

    calculate_active_cells_count();
    m_bulk_face_operations->update_face_x_velocities_distance_weighted();
    m_bulk_face_operations->update_face_y_velocities_distance_weighted();
    m_bulk_face_operations->update_face_x_viscosities();
    m_bulk_face_operations->update_face_y_viscosities();
    m_bulk_face_operations->update_face_x_pressures();
    m_bulk_face_operations->update_face_y_pressures();

    m_momentum_x_error = 1.0;
    m_momentum_y_error = 1.0;
    m_mass_imbalance = 1.0;
    m_outer_iterations_count = 0;
    while (m_momentum_x_error > m_velocity_u_tolerance || m_momentum_y_error > m_velocity_v_tolerance ||
           m_mass_imbalance > m_pressure_tolerance) {
        simple_iterate(SimulationType::Steady);
        std::cout << m_momentum_x_error << ", " << m_momentum_y_error << ", " << m_mass_imbalance << std::endl;
    }
    std::cout << "Converged in " << m_outer_iterations_count << " iterations" << std::endl;

    m_time_taken = m_timer->get_elapsed_time();

    // Save the results
    m_saver->open_and_clear_file();

    m_saver->write_domain_size();
    m_saver->write_grid_size();
    m_saver->write_execution_time(m_time_taken);

    m_saver->write_field(Field::VelocityU);
    m_saver->write_field(Field::VelocityV);
    m_saver->write_field(Field::Pressure);

    m_saver->close_file();
}
