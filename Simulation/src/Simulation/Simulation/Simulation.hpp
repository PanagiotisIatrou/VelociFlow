#pragma once

#include <string>
#include <memory>

#include "Meshing/Faces/BulkFaceOperations.hpp"
#include "Meshing/Nodes/BulkNodeOperations.hpp"
#include "Meshing/Mesh.hpp"
#include "Utilities/Saver.hpp"
#include "Utilities/Timer.hpp"

enum class VerboseType {None, Percentages, Residuals};

class Simulation {
protected:
    Mesh *m_mesh;
    std::unique_ptr<BulkNodeOperations> m_bulk_node_operations;
    std::unique_ptr<BulkFaceOperations> m_bulk_face_operations;

    double m_tolerance_velocity_x;
    double m_tolerance_velocity_y;
    double m_pressure_tolerance;
    double m_momentum_x_error = 0.0;
    double m_momentum_y_error = 0.0;
    double m_mass_imbalance = 0.0;
    double m_convection_diffusion_x_error = 0.0;
    double m_convection_diffusion_y_error = 0.0;
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

    // Verbosity
    VerboseType m_verbose_type;

    void calculate_active_cells_count();

    void simple_iterate(SimulationType simulation_type);

    void convection_diffusion_iterate(SimulationType simulation_type);

    double get_momentum_x_imbalance() const;

    void solve_x_momentum() const;

    double get_momentum_y_imbalance() const;

    void solve_y_momentum() const;

    double calculate_mass_imbalance();

    double get_pressure_correction_imbalance() const;

    void solve_pressure_correction() const;

    double get_dye_imbalance() const;

    double get_convection_diffusion_x_imbalance() const;

    void solve_convection_diffusion_x() const;

    double get_convection_diffusion_y_imbalance() const;

    void solve_convection_diffusion_y() const;

public:
    Simulation(Mesh *mesh, double tolerance_velocity_x, double tolerance_velocity_y, double pressure_tolerance, std::string output_file, VerboseType verbose_type);

    double get_time_taken() const;

    virtual void solve() = 0;

    void solve_dye() const;
};
