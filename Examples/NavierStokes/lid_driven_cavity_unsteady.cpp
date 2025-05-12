#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include <NavierStokesUnsteady.hpp>

const int grid_size_x = 150;
const int grid_size_y = 150;
const double domain_size_x = 1.0;
const double domain_size_y = 1.0;
const double velocity = 1.0;
const double viscosity = 1.0 / 1000.0;
const double density = 1.0;
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

            const FieldValues field_values{.velocity_x = 0.0, .velocity_y = 0.0, .pressure = 0.0, .dye = 0.0};
            mesh->set_node(i, j, field_values);
        }
    }

    // Add the moving lid
    for (int i = 0; i < grid_size_x; i++) {
        mesh->set_boundary_moving_wall_face(FaceSide::Y, i, grid_size_y, velocity);
    }

    // Link the nodes to their neighbouring nodes
    mesh->link_nodes();

    // Link the faces to their neighbouring nodes (and the opposite)
    mesh->link_nodes_faces();

    // Create the path for the output file
    const std::string folder = "";
    const std::string filename = "out-" + std::to_string(time(nullptr)) + ".txt";
    const std::string path = folder + filename;

    // // Run the simulation
    // const int extra_timesteps = 1000;
    // const std::string file = "../Results/Unsteady/out-1744278270.txt";
    // SimulatorContinuation simulation_continuation(file);
    // NavierStokesUnsteady simulation(mesh, &simulation_continuation, extra_timesteps, VerbosityType::Percentages);
    // simulation.solve();
    NavierStokesUnsteady simulation(mesh, density, viscosity, dt, timesteps, 1e-4, 1e-4, 1e-4, path, VerbosityType::Percentages);
    simulation.solve();

    std::cout << "Reached timestep " << simulation.get_reached_timesteps() << std::endl;
    std::cout << "Finished in " << simulation.get_time_taken() << " s" << std::endl;
    std::cout << "Saved output to file " << filename << std::endl;

    return 0;
}
