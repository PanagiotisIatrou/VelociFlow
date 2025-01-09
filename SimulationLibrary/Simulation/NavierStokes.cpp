#include "NavierStokes.hpp"

NavierStokes::NavierStokes(Mesh *mesh, const double density, const double viscosity, const double tolerance_momentum_x,
                           const double tolerance_momentum_y, const double tolerance_mass_imbalance,
                           const std::string output_file, const SimulationType simulation_type,
                           const VerbosityType verbosity_type)
    : Simulation(mesh, output_file, simulation_type, verbosity_type) {
    m_density = density;
    m_viscosity = viscosity;

    m_tolerance_momentum_x = tolerance_momentum_x;
    m_tolerance_momentum_y = tolerance_momentum_y;
    m_tolerance_mass_imbalance = tolerance_mass_imbalance;

    bool include_time = simulation_type == SimulationType::Unsteady;

    // Create the equations
    m_equation_momentum_x =
        std::make_unique<MomentumX>(mesh, Field::VelocityX, relaxation_velocity_x, ResidualType::Scaled,
                                    StoppingRule::Relative, NormType::L1, 1e-3, include_time);
    m_equation_momentum_y =
        std::make_unique<MomentumY>(mesh, Field::VelocityY, relaxation_velocity_y, ResidualType::Scaled,
                                    StoppingRule::Relative, NormType::L1, 1e-3, include_time);
    m_equation_pressure_correction = std::make_unique<PressureCorrection>(
        mesh, Field::PressureCorrection, 1.0, ResidualType::Scaled, StoppingRule::Absolute, NormType::L2, 1e-4);
    m_equation_dye = std::make_unique<Dye>(mesh, Field::Dye, 1.0, ResidualType::Scaled, StoppingRule::Absolute,
                                           NormType::L1, 1e-3, include_time);

    // Populate the mesh
    m_equation_momentum_x->populate_mesh();
    m_equation_momentum_y->populate_mesh();
    m_equation_pressure_correction->populate_mesh();
    m_equation_dye->populate_mesh();

    // Set density
    m_bulk_node_operations->set_density(m_density);
    m_bulk_face_operations->set_face_x_density(m_density);
    m_bulk_face_operations->set_face_y_density(m_density);

    // Set viscosity
    m_bulk_node_operations->set_viscosity(m_viscosity);
    m_bulk_face_operations->set_face_x_viscosity(m_viscosity);
    m_bulk_face_operations->set_face_y_viscosity(m_viscosity);

    // Initialize the face values
    m_bulk_face_operations->update_face_x_velocities_distance_weighted();
    m_bulk_face_operations->update_face_y_velocities_distance_weighted();
    m_bulk_face_operations->update_face_x_pressures();
    m_bulk_face_operations->update_face_y_pressures();
    m_bulk_face_operations->update_face_x_dye();
    m_bulk_face_operations->update_face_y_dye();
}

void NavierStokes::simple_iterate() {
    // Calculate the momentum coefficients
        m_equation_momentum_x->calculate_coefficients();
        m_equation_momentum_y->calculate_coefficients();

        // Calculate the momentum imbalance
        m_equation_momentum_x->calculate_imbalance();
        m_equation_momentum_y->calculate_imbalance();

        // Solve X and Y momentum equations
        m_equation_momentum_x->solve();
        m_equation_momentum_y->solve();

        // Calculate the face velocities
        m_bulk_face_operations->update_face_x_velocities_rhie_chow();
        m_bulk_face_operations->update_face_y_velocities_rhie_chow();

        // Calculate the pressure correction coefficients
        m_equation_pressure_correction->calculate_coefficients();

        // Calculate the mass imbalance
        m_equation_pressure_correction->calculate_mass_imbalance();

        // Solve pressure correction equation
        m_bulk_node_operations->reset_pressure_correction();
        m_equation_pressure_correction->solve();

        // Update the pressure correction on the faces
        m_bulk_face_operations->update_face_x_pressure_corrections();
        m_bulk_face_operations->update_face_y_pressure_corrections();

        // Correct the x and y node velocities
        m_bulk_node_operations->correct_node_velocity_x();
        m_bulk_node_operations->correct_node_velocity_y();

        // Correct the face x and y velocities
        m_bulk_face_operations->correct_face_x_velocity();
        m_bulk_face_operations->correct_face_y_velocity();

        // Correct the pressure on the nodes
        m_bulk_node_operations->correct_node_pressure();

        // Update the pressure on the faces
        m_bulk_face_operations->update_face_x_pressures();
        m_bulk_face_operations->update_face_y_pressures();

        m_outer_iterations_count++;
        m_equation_momentum_x->progress_iteration_counter();
        m_equation_momentum_y->progress_iteration_counter();
        m_equation_pressure_correction->progress_iteration_counter();
}
