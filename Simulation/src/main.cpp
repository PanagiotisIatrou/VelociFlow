#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include "MeshExamples.hpp"
#include "Simulation/Simulation/Meshing/Mesh.hpp"
#include "Simulation/SteadyConvectionDiffusionSimulation.hpp"
#include "Simulation/SteadySimulation.hpp"
#include "Simulation/UnsteadySimulation.hpp"

int main() {
    // Mesh *mesh = create_adv_diff_mesh(5.0, 1.0);
    Mesh *mesh = create_lid_driven_cavity_mesh(1.0, 1.0 / 1000.0, 1.0, 1.0);
    // Mesh *mesh = create_lid_driven_cavity_mesh(1.0, 1.0 / 3200.0, 1.0, 1.0);
    // Mesh *mesh = create_lid_driven_cavity_mesh(1.0, 1.0 / 10000, 1.0, 1.0);
    // Mesh *mesh = create_lid_driven_cavity_mesh(0.001, 0.001003, 998.2, 1.0);
    // Mesh *mesh = create_lid_driven_cavity_mesh(0.001, 0.000001003, 1.0, 1.0);
    // Mesh *mesh = create_double_lid_driven_cavity_mesh(10.0, 0.05);
    // Mesh *mesh = create_pipe_mesh(0.5, 0.05);
    // Mesh *mesh = create_pipe_obstacles_mesh(0.5, 0.05);
    // Mesh *mesh = create_box_mesh(1.0, 0.05);
    // Mesh *mesh = create_container_mesh(1.0, 0.05);
    // Mesh *mesh = create_circle_box_mesh(1.0, 0.01);
    // Mesh *mesh = create_rotating_circle_box_mesh(1.0, 0.5, 0.005);
    // Mesh *mesh = create_von_karman_mesh(1.0, 0.0005); // 500 - 0.0002, 200 - 0.0005, 120 - 0.000835, 65 - 0.00168
    // Mesh *mesh = create_kelvin_helmholtz_mesh(1.0, 0.0001);
    // Mesh *mesh = create_reynolds_mesh(1.0, 1.0 / 3000.0);

    // Create the path for the output file
    const std::string folder = "../../Results/Steady/";
    // const std::string folder = "../../Results/Unsteady/";
    const std::string filename = "out-" + std::to_string(time(nullptr)) + ".txt";
    const std::string path = folder + filename;

    const double tol = 1e-3;
    const double dt = 0.01;
    const int timesteps = 100;
    SteadySimulation simulation(mesh, 1.0, 1.0 / 1000.0, tol, tol, tol, path, VerboseType::Percentages);
    // UnsteadySimulation simulation(mesh, 1.0, 1.0 / 1000.0, dt, timesteps, tol, tol, tol, path, VerboseType::Percentages);
    // SteadyConvectionDiffusionSimulation simulation(mesh, 1.0, tol, tol, path, VerboseType::Residuals);
    simulation.solve();

    const double time_taken = simulation.get_time_taken();
    std::cout << "Finished in " << time_taken << " s" << std::endl;
    std::cout << "Saved output to file " << filename << std::endl;

    return 0;
}
