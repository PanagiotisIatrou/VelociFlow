#pragma once

inline Mesh *create_test_mesh() {
    const int N = 100;
    const int M = 100;
    const double domain_size_x = 1.0;
    const double domain_size_y = 1.0;
    const double density = 1.0;

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y);

    // Add the nodes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            mesh->set_node(i, j, 1.0, density);
        }
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    return mesh;
}