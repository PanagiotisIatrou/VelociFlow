#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#include "../src/UnsteadySimulation.hpp"
#include "../src/Simulation/Meshing/Faces/Interior/InteriorFaceX.hpp"
#include "../src/Simulation/Meshing/Faces/Interior/InteriorFaceY.hpp"

const int grid_size_x = 128;
const int grid_size_y = 128;
const double domain_size_x = 1.0;
const double domain_size_y = 1.0;
const double velocity = 1.0;
const double viscosity = 0.0001;
const double density = 1.0;
const double dt = 0.01;
const int timesteps = 100;

int main() {
    // Create the mesh
    Mesh *mesh = new Mesh(grid_size_x, grid_size_y, domain_size_x, domain_size_y);

    const double height = 20.0;

    // Add the nodes
    for (int i = 0; i < grid_size_x; i++) {
        for (int j = 0; j < grid_size_y; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            double velocity_x;
            if (std::abs(j - grid_size_y / 2.0) / (grid_size_y / 2.0) < 0.2 + 0.01 * std::sin(2.0 * M_PI * (static_cast<double>(i) / grid_size_x) * domain_size_x * 10.0)) {
                velocity_x = velocity;
                mesh->set_node(i, j, velocity_x, 0.0, 0.0, 1.0);
            } else {
                velocity_x = -velocity / 2.0;
                mesh->set_node(i, j, velocity_x, 0.0, 0.0, 0.0);
            }
        }
    }

    // Set periodic boundary conditions X
    for (int j = 0; j < grid_size_y; j++) {
        mesh->set_interior_face(FaceSide::X, 0, j);
        mesh->set_interior_face(FaceSide::X, grid_size_x - 1, j);
        mesh->set_interior_face(FaceSide::X, 1, j);
        InteriorFaceX *face = static_cast<InteriorFaceX *>(mesh->get_face_x(0, j));
        InteriorFaceX *face_left = static_cast<InteriorFaceX *>(mesh->get_face_x(grid_size_x - 1, j));
        InteriorFaceX *face_right = static_cast<InteriorFaceX *>(mesh->get_face_x(1, j));

        Node *first_node = mesh->get_node(0, j);
        Node *second_node = mesh->get_node(1, j);
        Node *last_node = mesh->get_node(grid_size_x - 1, j);
        Node *pre_last_node = mesh->get_node(grid_size_x - 2, j);

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
    for (int i = 0; i < grid_size_x; i++) {
        mesh->set_interior_face(FaceSide::Y, i, 0);
        mesh->set_interior_face(FaceSide::Y, i, grid_size_y - 1);
        mesh->set_interior_face(FaceSide::Y, i, 1);
        InteriorFaceY *face = static_cast<InteriorFaceY *>(mesh->get_face_y(i, 0));
        InteriorFaceY *face_left = static_cast<InteriorFaceY *>(mesh->get_face_y(i, grid_size_y - 1));
        InteriorFaceY *face_right = static_cast<InteriorFaceY *>(mesh->get_face_y(i, 1));

        Node *first_node = mesh->get_node(i, 0);
        Node *second_node = mesh->get_node(i, 1);
        Node *last_node = mesh->get_node(i, grid_size_y - 1);
        Node *pre_last_node = mesh->get_node(i, grid_size_y - 2);

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

    // Create the path for the output file
    const std::string folder = "../../Results/Unsteady/";
    const std::string filename = "out-" + std::to_string(time(nullptr)) + ".txt";
    const std::string path = folder + filename;

    // Run the simulation
    UnsteadySimulation simulation(mesh, density, viscosity, dt, timesteps, 1e-4, 1e-4, 1e-4, path, VerboseType::Percentages);
    simulation.solve();

    const double time_taken = simulation.get_time_taken();
    std::cout << "Finished in " << time_taken << " s" << std::endl;
    std::cout << "Saved output to file " << filename << std::endl;

    return 0;
}
