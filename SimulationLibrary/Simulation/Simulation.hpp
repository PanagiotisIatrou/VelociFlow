#pragma once

#include <memory>
#include <string>

#include "Meshing/Faces/BulkFaceOperations.hpp"
#include "Meshing/Mesh.hpp"
#include "Meshing/Nodes/BulkNodeOperations.hpp"
#include "Utilities/Saver.hpp"
#include "Utilities/Timer.hpp"
#include "Utilities/VerbosityHandler.hpp"

enum class SimulationType { Steady, Unsteady };

class Simulation {
protected:
    Mesh *m_mesh;

    std::unique_ptr<BulkNodeOperations> m_bulk_node_operations;
    std::unique_ptr<BulkFaceOperations> m_bulk_face_operations;

    int m_outer_iterations_count = 0;

    SimulationType m_simulation_type;

    // Timer
    std::unique_ptr<Timer> m_timer;
    double m_time_taken = 0.0;

    // Saver
    std::unique_ptr<Saver> m_saver;

    // Verbosity
    std::unique_ptr<VerbosityHandler> m_verbosity_handler;

public:
    virtual ~Simulation() = default;
    Simulation(Mesh *mesh, std::string output_file, SimulationType simulation_type, VerbosityType verbosity_type);

    double get_time_taken() const;

    virtual void solve() = 0;
};
