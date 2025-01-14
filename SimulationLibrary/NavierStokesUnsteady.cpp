#include "NavierStokesUnsteady.hpp"

#include <fstream>
#include <iostream>

NavierStokesUnsteady::NavierStokesUnsteady(Mesh *mesh, const double density, const double viscosity, const double dt,
                                           const int timesteps, const double tolerance_momentum_x,
                                           const double tolerance_momentum_y, const double tolerance_mass_imbalance,
                                           const std::string output_file, const VerbosityType verbosity_type)
    : NavierStokesSimulation(mesh, density, viscosity, tolerance_momentum_x, tolerance_momentum_y,
                             tolerance_mass_imbalance, output_file, SimulationType::Unsteady, verbosity_type) {
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

NavierStokesUnsteady::NavierStokesUnsteady(Mesh *mesh, SimulatorContinuation *simulator_continuation,
                                           const int extra_timesteps, const VerbosityType verbosity_type)
    : NavierStokesSimulation(mesh, simulator_continuation->density.value(), simulator_continuation->viscosity.value(),
                             simulator_continuation->tolerances[EquationType::MomentumX],
                             simulator_continuation->tolerances[EquationType::MomentumY],
                             simulator_continuation->tolerances[EquationType::PressureCorrection],
                             simulator_continuation->filename, SimulationType::Unsteady, verbosity_type) {
    m_continuing_simulation = true;

    // Set the normalization values
    m_equation_momentum_x->set_imbalance_normalization_factor(
        simulator_continuation->normalizations[EquationType::MomentumX]);
    m_equation_momentum_y->set_imbalance_normalization_factor(
        simulator_continuation->normalizations[EquationType::MomentumY]);
    m_equation_pressure_correction->set_imbalance_normalization_factor(
        simulator_continuation->normalizations[EquationType::PressureCorrection]);
    m_equation_pressure_correction->set_mass_imbalance_normalization_factor(
        simulator_continuation->mass_normalization.value());

    // Set the timesteps
    m_reached_timesteps = simulator_continuation->timesteps - 1;
    const int total_timesteps = simulator_continuation->timesteps + extra_timesteps - 1;
    m_timesteps = total_timesteps;
    m_verbosity_handler->enable_print_timesteps(total_timesteps);

    // Set dt
    m_dt = simulator_continuation->dt.value();
    m_mesh->set_dt(m_dt);
    m_bulk_node_operations->set_dt(m_dt);
    m_bulk_face_operations->set_face_x_dt(m_dt);
    m_bulk_face_operations->set_face_y_dt(m_dt);

    // Set the fields
    for (int i = 0; i < simulator_continuation->grid_size_x.value(); i++) {
        for (int j = 0; j < simulator_continuation->grid_size_y.value(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->set_field_value(Field::VelocityX, simulator_continuation->velocity_x[i][j]);
            node_P->set_field_value(Field::VelocityY, simulator_continuation->velocity_y[i][j]);
            node_P->set_field_value(Field::Pressure, simulator_continuation->pressure[i][j]);
            node_P->set_field_value(Field::Dye, simulator_continuation->dye[i][j]);
        }
    }

    // Save current field values as previous
    m_bulk_node_operations->update_node_previous_timestep_fields();
}

void NavierStokesUnsteady::solve() {
    start_ncurses();

    m_timer->start_timer();

    // Save the mesh settings and the initial state
    if (!m_continuing_simulation) {
        m_saver->open_and_clear_file();
        m_saver->write_simulation_name(SimulationName::NavierStokesUnsteady);
        m_saver->write_domain_size(m_mesh->get_domain_size_x(), m_mesh->get_domain_size_y());
        m_saver->write_grid_size(m_mesh->get_size_x(), m_mesh->get_size_y());
        m_saver->write_tolerance(EquationType::MomentumX, m_tolerance_momentum_x);
        m_saver->write_tolerance(EquationType::MomentumY, m_tolerance_momentum_y);
        m_saver->write_tolerance(EquationType::PressureCorrection, m_tolerance_mass_imbalance);
        m_saver->write_density(m_density);
        m_saver->write_viscosity(m_viscosity);
        m_saver->write_dt(m_dt);
        m_saver->write_field(Field::VelocityX);
        m_saver->write_field(Field::VelocityY);
        m_saver->write_field(Field::Pressure);
        m_saver->write_field(Field::Dye);
        m_saver->close_file();
    }

    bool has_quit = false;
    while (m_reached_timesteps < m_timesteps) {
        m_outer_iterations_count = 0;
        m_verbosity_handler->set_timesteps_count(m_reached_timesteps + 1);
        while (m_equation_momentum_x->get_imbalance() > m_tolerance_momentum_x ||
               m_equation_momentum_y->get_imbalance() > m_tolerance_momentum_y ||
               m_equation_pressure_correction->get_mass_imbalance() > m_tolerance_mass_imbalance) {
            simple_iterate();

            // Save the normalization values
            // TODO: Might fail if the first timestep converges too quickly (very rare)
            if (m_reached_timesteps == 0 &&
                m_outer_iterations_count == Equation::imbalance_normalization_iterations + 1) {
                m_saver->open_append_file();
                m_saver->write_normalization_values(EquationType::MomentumX, m_equation_momentum_x.get());
                m_saver->write_normalization_values(EquationType::MomentumY, m_equation_momentum_y.get());
                m_saver->write_normalization_values(EquationType::PressureCorrection,
                                                    m_equation_pressure_correction.get());
                m_saver->close_file();
            }

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

        m_reached_timesteps++;
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

int NavierStokesUnsteady::get_reached_timesteps() const {
    return m_reached_timesteps + 1;
}
