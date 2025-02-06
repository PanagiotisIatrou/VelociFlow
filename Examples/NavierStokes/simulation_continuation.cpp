#include <SimulatorContinuation.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "NavierStokesUnsteady.hpp"

const int grid_size_x = 100;
const int grid_size_y = 100;
const double domain_size_x = 1.0;
const double domain_size_y = 1.0;
const double velocity = 1.0;
const int extra_timesteps = 5;

int main() {
    // Create the mesh
    Mesh *mesh = new Mesh(grid_size_x, grid_size_y, domain_size_x, domain_size_y);

    // Add the nodes
    for (int i = 0; i < grid_size_x; i++) {
        for (int j = 0; j < grid_size_y; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            if (j == grid_size_y - 1 || j == grid_size_y - 2) {
                mesh->set_node(i, j, 0.0, 0.0, 0.0, 0.0);
            } else {
                mesh->set_node(i, j, 0.0, 0.0, 0.0, 0.0);
            }
        }
    }

    // Add the moving lid
    for (int i = 0; i < grid_size_x; i++) {
        for (int j = 0; j < grid_size_y + 1; j++) {
            if (j == grid_size_y) {
                mesh->set_boundary_moving_wall_face(FaceSide::Y, i, j, velocity);
            }
        }
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();


    // Run the simulation
    const std::string file = "../Results/Unsteady/out-1736787333.txt";
    SimulatorContinuation simulation_continuation(file);
    NavierStokesUnsteady simulation(mesh, &simulation_continuation, extra_timesteps, VerbosityType::Percentages);
    simulation.solve();

    std::cout << "Reached timestep " << simulation.get_reached_timesteps() << std::endl;
    std::cout << "Finished in " << simulation.get_time_taken() << " s" << std::endl;
    std::cout << "Saved output to file " << file << std::endl;

    return 0;
}
