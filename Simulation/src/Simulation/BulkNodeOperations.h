#pragma once
#include "Meshing/Mesh.hpp"

class BulkNodeOperations {
private:
    Mesh *m_mesh;
public:
    BulkNodeOperations(Mesh *mesh);

    void correct_node_pressure() const;

    void correct_node_velocity_u() const;

    void correct_node_velocity_v() const;
};
