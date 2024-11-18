#pragma once

#include <iostream>
#include <cmath>

#include "Simulation/Simulation/Meshing/Mesh.hpp"

inline Mesh *create_adv_diff_mesh(const double velocity, const double viscosity) {
    // Domain
    const int N = 100;
    const int M = 100;
    const double domain_size_x = 1.0;
    const double domain_size_y = 1.0;
    const double density = 1.0;

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y, density);

    // Add the nodes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            mesh->set_node(i, j, viscosity);
        }
    }

    // Add the moving lid
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M + 1; j++) {
            if (j == 0) {
                mesh->set_boundary_fixed_velocity_face(FaceSide::Y, i, j, -velocity, -velocity);
            }
            if (j == M) {
                mesh->set_boundary_fixed_velocity_face(FaceSide::Y, i, j, velocity, -velocity);
            }
        }
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    return mesh;
}

inline Mesh *create_lid_driven_cavity_mesh(const double velocity, const double viscosity) {
    // Domain
    const int N = 100;
    const int M = 100;
    const double domain_size_x = 1.0;
    const double domain_size_y = 1.0;
    const double density = 1.0;

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y, density);

    // Add the nodes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            mesh->set_node(i, j, viscosity);
        }
    }

    // Add the moving lid
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M + 1; j++) {
            if (j == M) {
                // double vel = std::sin((static_cast<double>(i) / N) * M_PI) * velocity;
                mesh->set_boundary_fixed_velocity_face(FaceSide::Y, i, j, velocity, 0.0);
            }
        }
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    return mesh;
}

inline Mesh *create_double_lid_driven_cavity_mesh(const double velocity, const double viscosity) {
    // Domain
    const int N = 100;
    const int M = 130;
    const double domain_size_x = 1.0;
    const double domain_size_y = 1.3;
    const double density = 1.0;

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y, density);

    // Add the nodes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            mesh->set_node(i, j, viscosity);
        }
    }

    // Add the moving lid
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M + 1; j++) {
            if (j == M) {
                mesh->set_boundary_fixed_velocity_face(FaceSide::Y, i, j, velocity, 0.0);
            }
        }
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    return mesh;
}

inline Mesh *create_pipe_mesh(double velocity, double viscosity) {
    // Domain
    const int N = 200;
    const int M = 50;
    const double domain_size_x = 4.0;
    const double domain_size_y = 1.0;
    const double density = 1.0;

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y, density);

    // Add the nodes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            mesh->set_node(i, j, viscosity);
        }
    }

    // Add the left and right boundaries
    for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < M; j++) {
            if (i == 0) {
                mesh->set_boundary_fixed_velocity_face(FaceSide::X, i, j, velocity, 0.0);
            } else if (i == N) {
                mesh->set_boundary_fixed_pressure_face(FaceSide::X, i, j, 0.0);
            }
        }
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    return mesh;
}

inline Mesh *create_pipe_obstacles_mesh(double velocity, double viscosity) {
    // Domain
    const int N = 160;
    const int M = 40;
    const double domain_size_x = 4.0;
    const double domain_size_y = 1.0;
    const double density = 1.0;

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y, density);

    // Add the nodes (and the obstacles)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (i > N / 4 - 3 && i < N / 4 + 3 && j > M / 3) {
                continue;
            }
            if (i > 2 * N / 4 - 3 && i < 2 * N / 4 + 3 && j < 2 * M / 3) {
                continue;
            }

            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            mesh->set_node(i, j, viscosity);
        }
    }

    // Add the left and right boundaries
    for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < M; j++) {
            if (i == 0) {
                mesh->set_boundary_fixed_velocity_face(FaceSide::X, i, j, velocity, 0.0);
            } else if (i == N) {
                mesh->set_boundary_fixed_pressure_face(FaceSide::X, i, j, 0.0);
            }
        }
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    return mesh;
}

inline Mesh *create_box_mesh(const double velocity, const double viscosity) {
    // Domain
    const int N = 200;
    const int M = 100;
    const double domain_size_x = 2.0;
    const double domain_size_y = 1.0;
    const double density = 1.0;

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y, density);

    // Add the nodes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            // No nodes for the 3rd quadrant
            if (i < N / 2 && j < M / 2) {
                continue;
            }

            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            mesh->set_node(i, j, viscosity);
        }
    }

    // Add the x boundary faces
    for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < M; j++) {
            if (i == 0 && j >= M / 2) {
                mesh->set_boundary_fixed_velocity_face(FaceSide::X, i, j, velocity, 0.0);
            } else if (i == N) {
                mesh->set_boundary_fixed_pressure_face(FaceSide::X, i, j, 0.0);
            }
        }
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    return mesh;
}

inline Mesh *create_container_mesh(const double velocity, const double viscosity) {
    // Domain
    const int N = 100;
    const int M = 100;
    const double domain_size_x = 1.0;
    const double domain_size_y = 1.0;
    const double density = 1.0;

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y, density);

    // Add the nodes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            mesh->set_node(i, j, viscosity);
        }
    }

    // Add the x boundary faces
    for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < M; j++) {
            if (i == 0 && j > M / 2 - 5 && j < M / 2 + 5) {
                mesh->set_boundary_fixed_pressure_face(FaceSide::X, i, j, 0.0);
            }
        }
    }

    // Add the y boundary faces
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M + 1; j++) {
            if (i > N / 2 - 5 && i < N / 2 + 5 && j == N) {
                mesh->set_boundary_fixed_velocity_face(FaceSide::Y, i, j, 0.0, -velocity);
            }
        }
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    return mesh;
}

inline Mesh *create_circle_box_mesh(const double velocity_inlet, const double viscosity) {
    // Domain
    const int N = 240;
    const int M = 60;
    const double domain_size_x = 4.0;
    const double domain_size_y = 1.0;
    const double density = 1.0;

    const double radius = 4.0;
    const double centerX = static_cast<double>(N) / 6;
    const double centerY = static_cast<double>(M) / 2;

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y, density);

    // Add the nodes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            // Form circle
            double distance = std::sqrt(std::pow(i - centerX, 2) + std::pow(j - centerY, 2));
            if (distance <= radius) {
                continue;
            }

            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            mesh->set_node(i, j, viscosity);
        }
    }

    // Add inlet and the outlet
    for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < M; j++) {
            if (i == 0) {
                mesh->set_boundary_fixed_velocity_face(FaceSide::X, i, j, velocity_inlet, 0.0);
            }
            else if (i == N) {
                mesh->set_boundary_fixed_pressure_face(FaceSide::X, i, j, 0.0);
            }
        }
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    return mesh;
}

inline Mesh *create_rotating_circle_box_mesh(const double velocity_inlet, const double velocity_circle, const double viscosity) {
    // Domain
    const int N = 250;
    const int M = 100;
    const double domain_size_x = 2.5;
    const double domain_size_y = 1.0;
    const double density = 1.0;

    const double radius = 10.0;
    const double centerX = static_cast<double>(N) / 3;
    const double centerY = static_cast<double>(M) / 2;

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y, density);

    // Add the nodes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            // Form circle
            double distance = std::sqrt(std::pow(i - centerX, 2) + std::pow(j - centerY, 2));
            if (distance <= radius) {
                // West face
                double local_x = i - 1 - centerX;
                double local_y = j - centerY;
                distance = std::sqrt(std::pow(local_x, 2) + std::pow(local_y, 2));
                if (distance > radius) {
                    const double velocity_x = local_y * (1 / radius) * velocity_circle;
                    const double velocity_y = -local_x * (1 / radius) * velocity_circle;
                    mesh->set_boundary_fixed_velocity_face(FaceSide::X, i, j, velocity_x, velocity_y);
                }

                // East face
                local_x = i + 1 - centerX;
                local_y = j - centerY;
                distance = std::sqrt(std::pow(local_x, 2) + std::pow(local_y, 2));
                if (distance > radius) {
                    const double velocity_x = local_y * (1 / radius) * velocity_circle;
                    const double velocity_y = -local_x * (1 / radius) * velocity_circle;
                    mesh->set_boundary_fixed_velocity_face(FaceSide::X, i + 1, j, velocity_x, velocity_y);
                }

                // South face
                local_x = i - centerX;
                local_y = j - 1 - centerY;
                distance = std::sqrt(std::pow(local_x, 2) + std::pow(local_y, 2));
                if (distance > radius) {
                    const double velocity_x = local_y * (1 / radius) * velocity_circle;
                    const double velocity_y = -local_x * (1 / radius) * velocity_circle;
                    mesh->set_boundary_fixed_velocity_face(FaceSide::Y, i, j, velocity_x, velocity_y);
                }

                // North face
                local_x = i - centerX;
                local_y = j + 1 - centerY;
                distance = std::sqrt(std::pow(local_x, 2) + std::pow(local_y, 2));
                if (distance > radius) {
                    const double velocity_x = local_y * (1 / radius) * velocity_circle;
                    const double velocity_y = -local_x * (1 / radius) * velocity_circle;
                    mesh->set_boundary_fixed_velocity_face(FaceSide::Y, i, j + 1, velocity_x, velocity_y);
                }

                continue;
            }

            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            mesh->set_node(i, j, viscosity);
        }
    }

    // Add inlet and the outlet
    for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < M; j++) {
            if (i == 0) {
                mesh->set_boundary_fixed_velocity_face(FaceSide::X, i, j, velocity_inlet, 0.0);
            }
            else if (i == N) {
                mesh->set_boundary_fixed_pressure_face(FaceSide::X, i, j, 0.0);
            }
        }
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    return mesh;
}
