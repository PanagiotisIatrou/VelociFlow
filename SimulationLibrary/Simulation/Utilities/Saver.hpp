#pragma once

#include <string>

#include "../Meshing/Mesh.hpp"
#include "Simulation/Equations/Equation.hpp"

class Saver {
public:
    Saver(Mesh *mesh, std::string path);

    Mesh *m_mesh;
    std::string m_path;
    FILE *m_file = nullptr;

    void open_and_clear_file();

    void open_append_file();

    void close_file();

    void write_simulation_name(SimulationName simulation_name) const;

    void write_domain_size(double size_x, double size_y) const;

    void write_grid_size(int size_x, int size_y) const;

    void write_execution_time(double execution_time) const;

    void write_dt(double dt) const;

    void write_density(double density) const;

    void write_viscosity(double viscosity) const;

    void write_tolerance(EquationType equation_type, Equation *equation) const;

    void write_timesteps_count(int timesteps) const;

    void write_field(Field field) const;

    void write_normalization_values(EquationType equation_type, Equation *equation) const;
};
