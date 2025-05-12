#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include <NavierStokesSteady.hpp>

const int grid_size_x = 400;
const int grid_size_y = 100;
const double domain_size_x = 4.0;
const double domain_size_y = 1.0;
const double velocity = 1.0;
const double viscosity = 0.025;
const double density = 1.0;

int main() {
    // Create the mesh
    Mesh *mesh = new Mesh(grid_size_x, grid_size_y, domain_size_x, domain_size_y);

    // Add the nodes (and the obstacles)
    for (int i = 0; i < grid_size_x; i++) {
        for (int j = 0; j < grid_size_y; j++) {
            if (i > grid_size_x / 4 - 7 && i < grid_size_x / 4 + 7 && j > grid_size_y / 3) {
                continue;
            }
            if (i > 2 * grid_size_x / 4 - 7 && i < 2 * grid_size_x / 4 + 7 && j < 2 * grid_size_y / 3) {
                continue;
            }

            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            const FieldValues values = {.velocity_x = 0.0, .velocity_y = 0.0, .pressure = 0.0};
            mesh->set_node(i, j, values);
        }
    }

    // Add the left and right boundaries
    for (int i = 0; i < grid_size_x + 1; i++) {
        for (int j = 0; j < grid_size_y; j++) {
            if (i == 0) {
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
    const std::string folder = "";
    const std::string filename = "out-" + std::to_string(time(nullptr)) + ".txt";
    const std::string path = folder + filename;

    // Run the simulation
    NavierStokesSteady simulation(mesh, density, viscosity, 1e-4, 1e-4, 1e-4, path, VerbosityType::Percentages);
    simulation.solve();

    std::cout << "Converged in " << simulation.get_outer_iterations_count() << " iterations" << std::endl;
    std::cout << "Finished in " << simulation.get_time_taken() << " s" << std::endl;
    std::cout << "Saved output to file " << filename << std::endl;

    return 0;
}
