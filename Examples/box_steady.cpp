#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include <NavierStokesSteady.hpp>

const int grid_size_x = 200;
const int grid_size_y = 100;
const double domain_size_x = 2.0;
const double domain_size_y = 1.0;
const double velocity = 1.0;
const double viscosity = 0.05;
const double density = 1.0;

int main() {
    // Create the mesh
    Mesh *mesh = new Mesh(grid_size_x, grid_size_y, domain_size_x, domain_size_y);

    // Add the nodes
    for (int i = 0; i < grid_size_x; i++) {
        for (int j = 0; j < grid_size_y; j++) {
            // No nodes for the 3rd quadrant
            if (i < grid_size_x / 2 && j < grid_size_y / 2) {
                continue;
            }

            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            mesh->set_node(i, j, 0.0, 0.0, 0.0, 0.0);
        }
    }

    // Add the x boundary faces
    for (int i = 0; i < grid_size_x + 1; i++) {
        for (int j = 0; j < grid_size_y; j++) {
            if (i == 0 && j >= grid_size_y / 2) {
                mesh->set_boundary_inlet_face(FaceSide::X, i, j, velocity, 0.0, 0.0);
            } else if (i == grid_size_x) {
                mesh->set_boundary_fixed_pressure_face(FaceSide::X, i, j, 0.0);
            }
        }
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    // Create the path for the output file
    const std::string folder = "../Results/Steady/";
    const std::string filename = "out-" + std::to_string(time(nullptr)) + ".txt";
    const std::string path = folder + filename;

    // Run the simulation
    NavierStokesSteady simulation(mesh, density, viscosity, 1e-4, 1e-4, 1e-4, path, VerbosityType::Percentages);
    simulation.solve();

    const double time_taken = simulation.get_time_taken();
    std::cout << "Finished in " << time_taken << " s" << std::endl;
    std::cout << "Saved output to file " << filename << std::endl;

    return 0;
}
