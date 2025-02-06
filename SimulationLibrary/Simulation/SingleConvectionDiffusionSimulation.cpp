#include "SingleConvectionDiffusionSimulation.hpp"

SingleConvectionDiffusionSimulation::SingleConvectionDiffusionSimulation(Mesh* mesh, const double viscosity,
                                                                         const double tolerance,
                                                                         const std::string output_file,
                                                                         const SimulationType simulation_type)
    : Simulation(mesh, output_file, simulation_type, VerbosityType::Residuals) {
    m_viscosity = viscosity;

    bool include_time = simulation_type == SimulationType::Unsteady;

    // Create the equations
    m_equation_single_convection_diffusion = std::make_unique<SingleConvectionDiffusionEquation>(
        mesh, Field::Phi, 1.0, ResidualType::Unscaled, StoppingRule::Absolute, NormType::L1, tolerance, include_time);

    // Populate all the nodes with the equation coefficients
    m_equation_single_convection_diffusion->populate_mesh();

    // Set viscosity
    m_bulk_node_operations->set_viscosity(m_viscosity);

    // Initialize the face values
    m_bulk_face_operations->update_face_x_phi();
    m_bulk_face_operations->update_face_y_phi();
    m_bulk_face_operations->update_face_x_velocities_distance_weighted();
    m_bulk_face_operations->update_face_y_velocities_distance_weighted();
}
