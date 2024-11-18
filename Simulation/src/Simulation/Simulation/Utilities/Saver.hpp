#pragma once

#include <string>

#include "../Meshing/Mesh.hpp"

class Saver {
public:
    Saver(Mesh *mesh, std::string path);

    Mesh *m_mesh;
    std::string m_path;
    FILE *m_file = nullptr;

    void open_and_clear_file();

    void open_append_file();

    void close_file();

    void write_domain_size() const;

    void write_grid_size() const;

    void write_execution_time(double execution_time) const;

    void write_dt() const;

    void write_timesteps_count(int timesteps) const;

    void write_field(Field field) const;
};
