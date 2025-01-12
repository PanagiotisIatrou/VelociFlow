#include "DiffusionSimulation.hpp"

DiffusionSimulation::DiffusionSimulation(Mesh* mesh, const double viscosity, const double tolerance_x,
                                         const double tolerance_y, const std::string output_file,
                                         const SimulationType simulation_type, const VerbosityType verbosity_type)
    : Simulation(mesh, output_file, simulation_type, verbosity_type) {
    m_viscosity = viscosity;
    m_tolerance_x = tolerance_x;
    m_tolerance_y = tolerance_y;

    bool include_time = simulation_type == SimulationType::Unsteady;

    // Create the equations
    m_equation_diffusion_x = std::make_unique<DiffusionX>(mesh, Field::VelocityX, 1.0, ResidualType::Scaled,
                                                          StoppingRule::Relative, NormType::L1, 1e-1, include_time);
    m_equation_diffusion_y = std::make_unique<DiffusionY>(mesh, Field::VelocityY, 1.0, ResidualType::Scaled,
                                                          StoppingRule::Relative, NormType::L1, 1e-1, include_time);

    // Populate all the nodes with the equation coefficients
    m_equation_diffusion_x->populate_mesh();
    m_equation_diffusion_y->populate_mesh();

    // Set viscosity
    m_bulk_node_operations->set_viscosity(m_viscosity);
    m_bulk_face_operations->set_face_x_viscosity(m_viscosity);
    m_bulk_face_operations->set_face_y_viscosity(m_viscosity);

    // Initialize the face velocities
    m_bulk_face_operations->update_face_x_velocities_distance_weighted();
    m_bulk_face_operations->update_face_y_velocities_distance_weighted();

    // Verbosity
    m_verbosity_handler->add_monitor(
        "X", [capture0 = m_equation_diffusion_x.get()] { return capture0->get_imbalance(); }, m_tolerance_x);
    m_verbosity_handler->add_monitor(
        "Y", [capture0 = m_equation_diffusion_y.get()] { return capture0->get_imbalance(); }, m_tolerance_y);
}

void DiffusionSimulation::iterate() {
    // Calculate the coefficients
    m_equation_diffusion_x->calculate_coefficients();
    m_equation_diffusion_y->calculate_coefficients();

    // Calculate the imbalance
    m_equation_diffusion_x->calculate_imbalance();
    m_equation_diffusion_y->calculate_imbalance();

    // Solve X and Y equations
    m_equation_diffusion_x->solve();
    m_equation_diffusion_y->solve();

    // Calculate the face velocities
    m_bulk_face_operations->update_face_x_velocities_distance_weighted();
    m_bulk_face_operations->update_face_y_velocities_distance_weighted();

    m_outer_iterations_count++;
}