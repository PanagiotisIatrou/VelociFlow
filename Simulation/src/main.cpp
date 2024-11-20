#include <iostream>
#include <string>
#include <chrono>
#include <fstream>

#include "MeshExamples.hpp"
#include "Simulation/Simulation/Meshing/Mesh.hpp"
#include "../tests/tests.hpp"
#include "Simulation/SteadySimulation.hpp"
#include "Simulation/UnsteadySimulation.hpp"

// TODO: What about convergence on Unsteady? Normalization of residuals ok?
// TODO: Create an unsteady simulation class

bool run_tests = false;
bool run_simulation = true;

int main() {
    if (run_tests) {
        run_all_tests();
    }

    if (!run_simulation) {
        return 0;
    }

    // Mesh *mesh = create_adv_diff_mesh(5.0, 1.0);
    Mesh *mesh = create_lid_driven_cavity_mesh(32.0, 0.01);
    // Mesh *mesh = create_double_lid_driven_cavity_mesh(10.0, 0.05);
    // Mesh *mesh = create_pipe_mesh(0.5, 0.05);
    // Mesh *mesh = create_pipe_obstacles_mesh(0.5, 0.05);
    // Mesh *mesh = create_box_mesh(1.0, 0.05);
    // Mesh *mesh = create_container_mesh(1.0, 0.05);
    // Mesh *mesh = create_circle_box_mesh(1.0, 0.01);
    // Mesh *mesh = create_rotating_circle_box_mesh(1.0, 0.5, 0.005);
    // Mesh *mesh = create_vok_karman_mesh(1.0, 0.0005);
    // Mesh *mesh = create_kelvin_helmholtz_mesh(1.0, 0.001);

    // Create the path for the output file
    const std::string folder = "../../Results/Steady/";
    // const std::string folder = "../../Results/Unsteady/";
    const std::string filename = "out-" + std::to_string(time(nullptr)) + ".txt";
    const std::string path = folder + filename;

    const double tol = 1e-5;
    SteadySimulation simulation(mesh, tol, tol, tol, path);
    const double dt = 0.1;
    const int timesteps = 50;
    // UnsteadySimulation simulation(mesh, dt, timesteps, tol, tol, tol, path);
    simulation.solve();

    const double time_taken = simulation.get_time_taken();
    std::cout << "Finished in " << time_taken << " s" << std::endl;
    std::cout << "Saved output to file " << filename << std::endl;

    return 0;
}
