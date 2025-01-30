#include <NavierStokesUnsteady.hpp>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

const int grid_size_x = 300;
const int grid_size_y = 300;
const double domain_size_x = 1.0;
const double domain_size_y = 1.0;
const double velocity = 1.0;
const double viscosity = 0.00001;
const double density = 1.0;
const double dt = 0.005;
const int timesteps = 1000;

int main() {
    // Create the mesh
    Mesh *mesh = new Mesh(grid_size_x, grid_size_y, domain_size_x, domain_size_y);

    // Add the nodes
    const double radius = 0.2;
    for (int i = 0; i < grid_size_x; i++) {
        for (int j = 0; j < grid_size_y; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            const double diff_x = static_cast<double>(i) / grid_size_x - 0.5;
            const double diff_y = static_cast<double>(j) / grid_size_y - 0.5;
            const double distance_from_center = std::sqrt(diff_x * diff_x + diff_y * diff_y);
            if (distance_from_center <= radius && distance_from_center > 0.0) {
                const double velocity_x = velocity * diff_y / distance_from_center;
                const double velocity_y = -velocity * diff_x / distance_from_center;
                mesh->set_node(i, j, velocity_x, velocity_y, 0.0, 1.0);
            } else if (distance_from_center == 0) {
                mesh->set_node(i, j, 0.0, 0.0, 0.0, 1.0);
            } else {
                mesh->set_node(i, j, 0.0, 0.0, 0.0, 0.0);
            }
        }
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
    // const std::string file = "../Results/Unsteady/out-1737553468.txt";
    // SimulatorContinuation simulation_continuation(file);
    // NavierStokesUnsteady simulation(mesh, &simulation_continuation, timesteps, VerbosityType::Percentages);
    NavierStokesUnsteady simulation(mesh, density, viscosity, dt, timesteps, 1e-4, 1e-4, 1e-4, path,
                                    VerbosityType::Percentages);
    simulation.solve();

    std::cout << "Reached timestep " << simulation.get_reached_timesteps() << std::endl;
    std::cout << "Finished in " << simulation.get_time_taken() << " s" << std::endl;
    std::cout << "Saved output to file " << filename << std::endl;

    return 0;
}
