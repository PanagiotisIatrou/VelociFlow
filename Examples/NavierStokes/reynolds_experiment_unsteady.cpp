#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include <NavierStokesUnsteady.hpp>

const int grid_size_x = 200;
const int grid_size_y = 50;
const double domain_size_x = 6.0;
const double domain_size_y = 1.0;
const double velocity = 1.0;
const double viscosity = 1.0 / 100000.0;
const double density = 1.0;
const double dt = 0.01;
const int timesteps = 100;

int main() {
    // Create the mesh
    Mesh *mesh = new Mesh(grid_size_x, grid_size_y, domain_size_x, domain_size_y);

    // Add the nodes
    for (int i = 0; i < grid_size_x; i++) {
        for (int j = 0; j < grid_size_y; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            const double noise_x = 0.02 * (rand() % 1000 / 1000.0) - 0.01;
            const double noise_y = 0.02 * (rand() % 1000 / 1000.0) - 0.01;
            mesh->set_node(i, j, noise_x, noise_y, 0.0, 0.0);
        }
    }

    // Add the left and right boundaries
    for (int i = 0; i < grid_size_x + 1; i++) {
        for (int j = 0; j < grid_size_y; j++) {
            if (i == 0) {
                if (std::abs(j - grid_size_y / 2) <= 1) {
                    mesh->set_boundary_inlet_face(FaceSide::X, i, j, velocity, 0.0, 1.0);
                } else {
                    mesh->set_boundary_inlet_face(FaceSide::X, i, j, velocity, 0.0, 0.0);
                }
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
    int extra_timesteps = 500;
    const std::string file = "../Results/Unsteady/out-1738169391.txt";
    SimulatorContinuation simulation_continuation(file);
    NavierStokesUnsteady simulation(mesh, &simulation_continuation, extra_timesteps, VerbosityType::Percentages);
    // NavierStokesUnsteady simulation(mesh, density, viscosity, dt, timesteps, 1e-4, 1e-4, 1e-4, path, VerbosityType::Percentages);
    simulation.solve();

    std::cout << "Reached timestep " << simulation.get_reached_timesteps() << std::endl;
    std::cout << "Finished in " << simulation.get_time_taken() << " s" << std::endl;
    std::cout << "Saved output to file " << filename << std::endl;

    return 0;
}
