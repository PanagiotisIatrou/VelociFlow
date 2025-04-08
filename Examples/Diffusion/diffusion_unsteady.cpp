#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include <DiffusionUnsteady.hpp>

const int grid_size_x = 30;
const int grid_size_y = 30;
const double domain_size_x = 1.0;
const double domain_size_y = 1.0;
const double phi = 1.0;
const double viscosity = 0.01;
const double dt = 0.01;
const int timesteps = 1000;

int main() {
    // Create the mesh
    Mesh *mesh = new Mesh(grid_size_x, grid_size_y, domain_size_x, domain_size_y);

    // Add the nodes
    for (int i = 0; i < grid_size_x; i++) {
        for (int j = 0; j < grid_size_y; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            const FieldValues field_values{.phi = 0.0};
            mesh->set_node(i, j, field_values);
        }
    }

    // Add the faces
    for (int i = 0; i < grid_size_x; i++) {
        mesh->set_boundary_fixed_value_face(FaceSide::Y, i, 0, 0.0);
        mesh->set_boundary_fixed_value_face(FaceSide::Y, i, grid_size_y, phi);
    }
    for (int j = 0; j < grid_size_y; j++) {
        mesh->set_boundary_fixed_value_face(FaceSide::X, 0, j, 0.0);
        mesh->set_boundary_fixed_value_face(FaceSide::X, grid_size_x, j, 0.0);
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    // Create the path for the output file
    const std::string folder = "../Results/Unsteady/";
    const std::string filename = "out-" + std::to_string(time(nullptr)) + ".txt";
    const std::string path = folder + filename;

    // Run the simulation
    DiffusionUnsteady simulation(mesh, viscosity, dt, timesteps, 1e-4, path);
    simulation.solve();

    std::cout << "Reached timestep " << simulation.get_reached_timesteps() << std::endl;
    std::cout << "Finished in " << simulation.get_time_taken() << " s" << std::endl;
    std::cout << "Saved output to file " << filename << std::endl;

    return 0;
}
