#pragma once

#include <iostream>
#include <cmath>

#include "Simulation/Simulation/Meshing/Mesh.hpp"
#include "Simulation/Simulation/Meshing/Faces/Interior/InteriorFaceX.hpp"
#include "Simulation/Simulation/Meshing/Faces/Interior/InteriorFaceY.hpp"

inline Mesh *create_adv_diff_mesh(const double velocity, const double viscosity) {
    // Domain
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

            mesh->set_node(i, j, viscosity, density, 0.0, 0.0, 0.0, 0.0);
        }
    }

    // Add the moving lid
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M + 1; j++) {
            if (j == 0) {
                mesh->set_boundary_inlet_face(FaceSide::Y, i, j, -velocity, -velocity, 0.0);
            }
            if (j == M) {
                mesh->set_boundary_inlet_face(FaceSide::Y, i, j, velocity, -velocity, 0.0);
            }
        }
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    return mesh;
}

inline Mesh *create_lid_driven_cavity_mesh(const double velocity, const double viscosity, const double density, const double size) {
    // Domain
    const int N = 50;
    const int M = 50;
    const double domain_size_x = size;
    const double domain_size_y = size;

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y);

    // Add the nodes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            if (j > M / 2) {
                mesh->set_node(i, j, viscosity, density, 0.0, 0.0, 0.0, 0.0);
            } else {
                mesh->set_node(i, j, viscosity, density, 0.0, 0.0, 0.0, 1.0);
            }
        }
    }

    // Add the moving lid
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M + 1; j++) {
            if (j == M) {
                mesh->set_boundary_moving_wall_face(FaceSide::Y, i, j, velocity);
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

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y);

    // Add the nodes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            mesh->set_node(i, j, viscosity, density, 0.0, 0.0, 0.0, 0.0);
        }
    }

    // Add the moving lid
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M + 1; j++) {
            if (j == M) {
                mesh->set_boundary_moving_wall_face(FaceSide::Y, i, j, velocity);
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
    const int N = 150;
    const int M = 50;
    const double domain_size_x = 3.0;
    const double domain_size_y = 1.0;
    const double density = 1.0;

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y);

    // Add the nodes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            mesh->set_node(i, j, viscosity, density, 0.0, 0.0, 0.0, 0.0);
        }
    }

    // Add the left and right boundaries
    for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < M; j++) {
            if (i == 0) {
                mesh->set_boundary_inlet_face(FaceSide::X, i, j, velocity, 0.0, 0.0);
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

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y);

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

            mesh->set_node(i, j, viscosity, density, 0.0, 0.0, 0.0, 0.0);
        }
    }

    // Add the left and right boundaries
    for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < M; j++) {
            if (i == 0) {
                mesh->set_boundary_inlet_face(FaceSide::X, i, j, velocity, 0.0, 0.0);
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

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y);

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

            mesh->set_node(i, j, viscosity, density, 0.0, 0.0, 0.0, 0.0);
        }
    }

    // Add the x boundary faces
    for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < M; j++) {
            if (i == 0 && j >= M / 2) {
                mesh->set_boundary_inlet_face(FaceSide::X, i, j, velocity, 0.0, 0.0);
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

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y);

    // Add the nodes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            mesh->set_node(i, j, viscosity, density, 0.0, 0.0, 0.0, 0.0);
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
                mesh->set_boundary_inlet_face(FaceSide::Y, i, j, 0.0, -velocity, 0.0);
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

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y);

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

            mesh->set_node(i, j, viscosity, density, 0.0, 0.0, 0.0, 0.0);
        }
    }

    // Add inlet and the outlet
    for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < M; j++) {
            if (i == 0) {
                mesh->set_boundary_inlet_face(FaceSide::X, i, j, velocity_inlet, 0.0, 0.0);
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
    const int N = 200;
    const int M = 80;
    const double domain_size_x = 2.5;
    const double domain_size_y = 1.0;
    const double density = 1.0;

    const double radius = 8.0;
    const double centerX = static_cast<double>(N) / 5;
    const double centerY = static_cast<double>(M) / 2;

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y);

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
                    mesh->set_boundary_inlet_face(FaceSide::X, i, j, velocity_x, velocity_y, 0.0);
                }

                // East face
                local_x = i + 1 - centerX;
                local_y = j - centerY;
                distance = std::sqrt(std::pow(local_x, 2) + std::pow(local_y, 2));
                if (distance > radius) {
                    const double velocity_x = local_y * (1 / radius) * velocity_circle;
                    const double velocity_y = -local_x * (1 / radius) * velocity_circle;
                    mesh->set_boundary_inlet_face(FaceSide::X, i + 1, j, velocity_x, velocity_y, 0.0);
                }

                // South face
                local_x = i - centerX;
                local_y = j - 1 - centerY;
                distance = std::sqrt(std::pow(local_x, 2) + std::pow(local_y, 2));
                if (distance > radius) {
                    const double velocity_x = local_y * (1 / radius) * velocity_circle;
                    const double velocity_y = -local_x * (1 / radius) * velocity_circle;
                    mesh->set_boundary_inlet_face(FaceSide::Y, i, j, velocity_x, velocity_y, 0.0);
                }

                // North face
                local_x = i - centerX;
                local_y = j + 1 - centerY;
                distance = std::sqrt(std::pow(local_x, 2) + std::pow(local_y, 2));
                if (distance > radius) {
                    const double velocity_x = local_y * (1 / radius) * velocity_circle;
                    const double velocity_y = -local_x * (1 / radius) * velocity_circle;
                    mesh->set_boundary_inlet_face(FaceSide::Y, i, j + 1, velocity_x, velocity_y, 0.0);
                }

                continue;
            }

            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            mesh->set_node(i, j, viscosity, density, 0.0, 0.0, 0.0, 0.0);
        }
    }

    // Add inlet and the outlet
    for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < M; j++) {
            if (i == 0) {
                mesh->set_boundary_inlet_face(FaceSide::X, i, j, velocity_inlet, 0.0, 0.0);
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

inline Mesh *create_von_karman_mesh(const double velocity_inlet, const double viscosity) {
    // Domain
    const int N = 100;
    const int M = 50;
    const double domain_size_x = 2.0;
    const double domain_size_y = 1;
    const double density = 1.0;

    const double length = 5.0;
    const double centerX = N / 5;
    const double centerY = M / 2;

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y);

    // Add the nodes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            // Form box
            if (i > centerX - length / 2.0 && i < centerX + length / 2.0 &&
                j > centerY - length / 2.0 && j < centerY + length / 2.0) {
                continue;
            }

            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            mesh->set_node(i, j, viscosity, density, 0.0, 0.0, 0.0, 0.0);
        }
    }

    // Add inlet and the outlet
    for (int j = 0; j < M; j++) {
        if (std::abs(j - M / 2) <= 1) {
            mesh->set_boundary_inlet_face(FaceSide::X, 0, j, velocity_inlet, 0.0, 1.0);
        } else {
            mesh->set_boundary_inlet_face(FaceSide::X, 0, j, velocity_inlet, 0.0, 0.0);
        }
        mesh->set_boundary_fixed_pressure_face(FaceSide::X, N, j, 0.0);
    }

    // Add Slip boundary conditions to the top and bottom
    for (int i = 0; i < N; i++) {
        mesh->set_boundary_slip_face(FaceSide::Y, i, 0);
        mesh->set_boundary_slip_face(FaceSide::Y, i, M);
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    return mesh;
}

inline Mesh *create_kelvin_helmholtz_mesh(const double velocity_inlet, const double viscosity) {
    // Domain
    const int N = 128;
    const int M = 128;
    const double domain_size_x = 1.0;
    const double domain_size_y = 1.0;
    const double density = 1.0;

    const double height = 20.0;

    Mesh *mesh = new Mesh(N, M, domain_size_x, domain_size_y);

    // Add the nodes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            double velocity_u;
            if (std::abs(j - M / 2.0) / (M / 2.0) < 0.2 + 0.02 * std::sin(2.0 * M_PI * (static_cast<double>(i) / N) * domain_size_x * 10.0)) {
                velocity_u = velocity_inlet;
            } else {
                velocity_u = -velocity_inlet / 2.0;
            }
            mesh->set_node(i, j, viscosity, density, velocity_u, 0.0, 0.0, 0.0);
        }
    }

    // Set periodic boundary conditions X
    for (int j = 0; j < M; j++) {
        mesh->set_interior_face(FaceSide::X, 0, j);
        mesh->set_interior_face(FaceSide::X, N - 1, j);
        mesh->set_interior_face(FaceSide::X, 1, j);
        InteriorFaceX *face = static_cast<InteriorFaceX *>(mesh->get_face_x(0, j));
        InteriorFaceX *face_left = static_cast<InteriorFaceX *>(mesh->get_face_x(N - 1, j));
        InteriorFaceX *face_right = static_cast<InteriorFaceX *>(mesh->get_face_x(1, j));

        Node *first_node = mesh->get_node(0, j);
        Node *second_node = mesh->get_node(1, j);
        Node *last_node = mesh->get_node(N - 1, j);
        Node *pre_last_node = mesh->get_node(N - 2, j);

        // Faces

        face->set_node_neighbour(last_node, FaceXSide::West);
        face->set_node_neighbour(first_node, FaceXSide::East);

        face_left->set_node_neighbour(pre_last_node, FaceXSide::West);
        face_left->set_node_neighbour(last_node, FaceXSide::East);

        face_right->set_node_neighbour(first_node, FaceXSide::West);
        face_right->set_node_neighbour(second_node, FaceXSide::East);

        // Nodes

        // pre_last_node
        pre_last_node->set_neighbouring_face(face_left, Direction::East);
        pre_last_node->set_neighbouring_face(face, Direction::EastEast);

        pre_last_node->set_neighbouring_node(last_node, Direction::East);
        pre_last_node->set_neighbouring_node(first_node, Direction::EastEast);

        // last_node
        last_node->set_neighbouring_face(face_left, Direction::West);
        last_node->set_neighbouring_face(face, Direction::East);
        last_node->set_neighbouring_face(face_right, Direction::EastEast);

        last_node->set_neighbouring_node(pre_last_node, Direction::West);
        last_node->set_neighbouring_node(first_node, Direction::East);
        last_node->set_neighbouring_node(second_node, Direction::EastEast);

        // first_node
        first_node->set_neighbouring_face(face_left, Direction::WestWest);
        first_node->set_neighbouring_face(face, Direction::West);
        first_node->set_neighbouring_face(face_right, Direction::East);

        first_node->set_neighbouring_node(pre_last_node, Direction::WestWest);
        first_node->set_neighbouring_node(last_node, Direction::West);
        first_node->set_neighbouring_node(second_node, Direction::East);

        // second_node
        second_node->set_neighbouring_face(face, Direction::WestWest);
        second_node->set_neighbouring_face(face_right, Direction::West);

        second_node->set_neighbouring_node(last_node, Direction::WestWest);
        second_node->set_neighbouring_node(first_node, Direction::West);
    }

    // Set periodic boundary conditions Y
    for (int i = 0; i < N; i++) {
        mesh->set_interior_face(FaceSide::Y, i, 0);
        mesh->set_interior_face(FaceSide::Y, i, M - 1);
        mesh->set_interior_face(FaceSide::Y, i, 1);
        InteriorFaceY *face = static_cast<InteriorFaceY *>(mesh->get_face_y(i, 0));
        InteriorFaceY *face_left = static_cast<InteriorFaceY *>(mesh->get_face_y(i, M - 1));
        InteriorFaceY *face_right = static_cast<InteriorFaceY *>(mesh->get_face_y(i, 1));

        Node *first_node = mesh->get_node(i, 0);
        Node *second_node = mesh->get_node(i, 1);
        Node *last_node = mesh->get_node(i, M - 1);
        Node *pre_last_node = mesh->get_node(i, M - 2);

        // Faces

        face->set_node_neighbour(last_node, FaceYSide::South);
        face->set_node_neighbour(first_node, FaceYSide::North);

        face_left->set_node_neighbour(pre_last_node, FaceYSide::South);
        face_left->set_node_neighbour(last_node, FaceYSide::North);

        face_right->set_node_neighbour(first_node, FaceYSide::South);
        face_right->set_node_neighbour(second_node, FaceYSide::North);

        // Nodes

        // pre_last_node
        pre_last_node->set_neighbouring_face(face_left, Direction::North);
        pre_last_node->set_neighbouring_face(face, Direction::NorthNorth);

        pre_last_node->set_neighbouring_node(last_node, Direction::North);
        pre_last_node->set_neighbouring_node(first_node, Direction::NorthNorth);

        // last_node
        last_node->set_neighbouring_face(face_left, Direction::South);
        last_node->set_neighbouring_face(face, Direction::North);
        last_node->set_neighbouring_face(face_right, Direction::NorthNorth);

        last_node->set_neighbouring_node(pre_last_node, Direction::South);
        last_node->set_neighbouring_node(first_node, Direction::North);
        last_node->set_neighbouring_node(second_node, Direction::NorthNorth);

        // first_node
        first_node->set_neighbouring_face(face_left, Direction::SouthSouth);
        first_node->set_neighbouring_face(face, Direction::South);
        first_node->set_neighbouring_face(face_right, Direction::North);

        first_node->set_neighbouring_node(pre_last_node, Direction::SouthSouth);
        first_node->set_neighbouring_node(last_node, Direction::South);
        first_node->set_neighbouring_node(second_node, Direction::North);

        // second_node
        second_node->set_neighbouring_face(face, Direction::SouthSouth);
        second_node->set_neighbouring_face(face_right, Direction::South);

        second_node->set_neighbouring_node(last_node, Direction::SouthSouth);
        second_node->set_neighbouring_node(first_node, Direction::South);
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    return mesh;
}
