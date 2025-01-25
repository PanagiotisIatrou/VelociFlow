#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#include <NavierStokesUnsteady.hpp>
#include <Simulation/Meshing/Faces/Interior/InteriorFace.hpp>

const int grid_size_x = 300;
const int grid_size_y = 300;
const double domain_size_x = 1.0;
const double domain_size_y = 1.0;
const double velocity = 1.0;
const double viscosity = 0.0001;
const double density = 1.0;
const double dt = 0.00025;
const int timesteps = 1000;

int main() {
    // Create the mesh
    Mesh *mesh = new Mesh(grid_size_x, grid_size_y, domain_size_x, domain_size_y);

    const double height = 0.2;

    // Add the nodes
    for (int i = 0; i < grid_size_x; i++) {
        for (int j = 0; j < grid_size_y; j++) {
            if (mesh->get_node(i, j) != nullptr) {
                std::cout << "! Reallocation !" << std::endl;
            }

            double velocity_x;
            if (std::abs(j - grid_size_y / 2.0) / (grid_size_y / 2.0) < height + 0.01 * std::sin(2.0 * M_PI * (static_cast<double>(i) / grid_size_x) * domain_size_x * 10.0)) {
                velocity_x = velocity;
                mesh->set_node(i, j, velocity_x, 0.0, 0.0, 1.0);
            } else {
                velocity_x = -velocity / 2.0;
                mesh->set_node(i, j, velocity_x, 0.0, 0.0, 0.0);
            }
        }
    }

    // Set periodic boundary conditions on the x-axis
    mesh->set_boundary_periodic_side(FaceSide::X);

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    // Create the path for the output file
    const std::string folder = "../Results/Unsteady/";
    const std::string filename = "out-" + std::to_string(time(nullptr)) + ".txt";
    const std::string path = folder + filename;

    // Run the simulation
    const std::string file = "../Results/Unsteady/out-1737469287.txt";
    SimulatorContinuation simulation_continuation(file);
    NavierStokesUnsteady simulation(mesh, &simulation_continuation, timesteps, VerbosityType::Percentages);
    // NavierStokesUnsteady simulation(mesh, density, viscosity, dt, timesteps, 1e-4, 1e-4, 1e-4, path, VerbosityType::Percentages);
    simulation.solve();

    std::cout << "Reached " << simulation.get_reached_timesteps() << " / " << timesteps << " timesteps" << std::endl;
    std::cout << "Finished in " << simulation.get_time_taken() << " s" << std::endl;
    std::cout << "Saved output to file " << filename << std::endl;

    return 0;
}
