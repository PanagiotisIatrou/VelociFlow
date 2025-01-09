#include "Simulation.hpp"

Simulation::Simulation(Mesh *mesh, const std::string output_file, const SimulationType simulation_type,
                       const VerbosityType verbosity_type) {
    m_mesh = mesh;
    m_bulk_node_operations = std::make_unique<BulkNodeOperations>(mesh);
    m_bulk_face_operations = std::make_unique<BulkFaceOperations>(mesh);
    m_timer = std::make_unique<Timer>();
    m_saver = std::make_unique<Saver>(mesh, output_file);
    m_verbosity_handler = std::make_unique<VerbosityHandler>(verbosity_type);
    m_simulation_type = simulation_type;
}

double Simulation::get_time_taken() const {
    return m_time_taken;
}
