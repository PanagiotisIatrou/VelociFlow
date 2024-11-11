#pragma once

#include <string>

#include "../Meshing/Mesh.hpp"

class Saver {
public:
    static void save_mesh_data(const std::string &path, double execution_time, const Mesh *mesh);
};
