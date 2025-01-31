#include "DiffusionSimulation.hpp"

DiffusionSimulation::DiffusionSimulation(Mesh* mesh, const double viscosity, const double tolerance_x,
                                         const double tolerance_y, const std::string output_file,
                                         const SimulationType simulation_type)
    : Simulation(mesh, output_file, simulation_type, VerbosityType::None) {
    m_viscosity = viscosity;

    bool include_time = simulation_type == SimulationType::Unsteady;

    // Create the equations
    m_equation_diffusion_x =
        std::make_unique<DiffusionX>(mesh, Field::VelocityX, 1.0, ResidualType::Scaled, StoppingRule::Absolute,
                                     NormType::L1, tolerance_x, include_time);
    m_equation_diffusion_y =
        std::make_unique<DiffusionY>(mesh, Field::VelocityY, 1.0, ResidualType::Scaled, StoppingRule::Absolute,
                                     NormType::L1, tolerance_y, include_time);

    // Populate all the nodes with the equation coefficients
    m_equation_diffusion_x->populate_mesh();
    m_equation_diffusion_y->populate_mesh();

    // Set viscosity
    m_bulk_node_operations->set_viscosity(m_viscosity);

    // Initialize the face velocities
    m_bulk_face_operations->update_face_x_velocities_distance_weighted();
    m_bulk_face_operations->update_face_y_velocities_distance_weighted();
}

void DiffusionSimulation::iterate() {
    // Calculate the coefficients
    m_equation_diffusion_x->calculate_coefficients();
    m_equation_diffusion_y->calculate_coefficients();

    // Calculate the imbalance
    m_equation_diffusion_x->calculate_imbalance();
    m_equation_diffusion_y->calculate_imbalance();

    // Solve X Diffusion equation
    m_equation_diffusion_x->solve();
    std::cout << "Solved X Diffusion equation" << std::endl;

    // Solve Y Diffusion equation
    m_equation_diffusion_y->solve();
    std::cout << "Solved Y Diffusion equation" << std::endl;
}