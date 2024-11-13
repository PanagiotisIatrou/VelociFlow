#include <iostream>
#include <string>
#include <chrono>
#include <fstream>

#include "MeshExamples.hpp"
#include "Simulation/Meshing/Mesh.hpp"
#include "Simulation/SteadySimulation.hpp"
#include "../tests/tests.hpp"

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
    Mesh *mesh = create_lid_driven_cavity_mesh(10.0, 0.05);
    // Mesh *mesh = create_double_lid_driven_cavity_mesh(10.0, 0.05);
    // Mesh *mesh = create_pipe_mesh(0.5, 0.05);
    // Mesh *mesh = create_pipe_obstacles_mesh(0.5, 0.05);
    // Mesh *mesh = create_box_mesh(1.0, 0.05);
    // Mesh *mesh = create_container_mesh(1.0, 0.05);
    // Mesh *mesh = create_circle_mesh(5.0, 15.0, 0.05);
    // Mesh *mesh = create_circle_box_mesh(5.0, 5.0, 0.05);

    const double tol = 1e-4;
    SteadySimulation simulation(mesh, tol, tol, tol);
    simulation.solve();

    const double time_taken = simulation.get_time_taken();
    std::cout << "Finished in " << time_taken << " s" << std::endl;

    // Save the results
    const std::string folder = "../../Results/Steady/";
    const std::string filename = "out-" + std::to_string(time(nullptr)) + ".txt";
    const std::string path = folder + filename;
    simulation.save_results(path);
    std::cout << "Saved output to file " << filename << std::endl;

    return 0;
}
