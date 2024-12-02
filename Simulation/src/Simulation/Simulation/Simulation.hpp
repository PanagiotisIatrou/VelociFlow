#pragma once

#include <string>
#include <memory>

#include "BulkFaceOperations.hpp"
#include "BulkNodeOperations.hpp"
#include "Meshing/Mesh.hpp"
#include "Utilities/Saver.hpp"
#include "Utilities/Timer.hpp"

class Simulation {
protected:
    Mesh *m_mesh;
    std::unique_ptr<BulkNodeOperations> m_bulk_node_operations;
    std::unique_ptr<BulkFaceOperations> m_bulk_face_operations;

    double m_velocity_u_tolerance;
    double m_velocity_v_tolerance;
    double m_pressure_tolerance;
    double m_momentum_x_error = 0.0;
    double m_momentum_y_error = 0.0;
    double m_mass_imbalance = 0.0;
    int m_active_cells_count = 0;
    int m_outer_iterations_count = 0;

    // Timer
    std::unique_ptr<Timer> m_timer;
    double m_time_taken = 0.0;

    // Saver
    std::unique_ptr<Saver> m_saver;

    // Residual normalization
    const int residual_normalization_iterations = 5;
    bool can_update_mass_imbalance_residual_normalization_factor = true;
    double m_mass_imbalance_residual_normalization_factor = 0.0;

    void calculate_active_cells_count();

    void simple_iterate(SimulationType simulation_type);

    void calculate_momentum_x_imbalance();

    void solve_x_momentum() const;

    void calculate_momentum_y_imbalance();

    void solve_y_momentum() const;

    void calculate_mass_imbalance();

    void solve_pressure_correction() const;

public:
    Simulation(Mesh *mesh, double velocity_u_tolerance, double velocity_v_tolerance, double pressure_tolerance, std::string output_file);

    double get_time_taken() const;

    virtual void solve() = 0;
};
