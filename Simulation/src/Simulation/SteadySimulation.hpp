#pragma once

#include <string>
#include <memory>

#include "BulkFaceOperations.h"
#include "BulkNodeOperations.h"
#include "Meshing/Mesh.hpp"
#include "Utilities/Timer.h"

class SteadySimulation {
private:
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

    std::unique_ptr<Timer> m_timer;
    double m_time_taken = 0.0;

    void calculate_active_cells_count();

    void iterate();

    void solve_x_momentum();

    void solve_y_momentum();

    void calculate_mass_imbalance();

    void solve_pressure_correction() const;

public:
    SteadySimulation(Mesh *mesh, double velocity_u_tolerance, double velocity_v_tolerance, double pressure_tolerance);

    double get_time_taken() const;

    void solve();

    void save_results(const std::string &path) const;

    void calculate_momentum_coefficients(VelocityComponent velocity_component) const;

    void calculate_pressure_coefficients() const;
};
