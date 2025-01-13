#include "SimulatorContinuation.hpp"

#include <fstream>
#include <sstream>

#include "NavierStokesUnsteady.hpp"

SimulatorContinuation::SimulatorContinuation(const std::string filename) {
    this->filename = filename;

    // Open the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "File " << filename << " not found" << std::endl;
        exit(1);
    }

    // Read the file
    std::string line;
    while (getline(file, line)) {
        if (line.find("simulation_name") != std::string::npos) {
            getline(file, line);
            simulation_name = str_to_simulation_name(line);
        } else if (line.find("grid_size") != std::string::npos) {
            getline(file, line);
            std::stringstream ss(line);
            std::vector<std::string> tokens;
            while (ss.good()) {
                std::string token;
                getline(ss, token, ',');
                tokens.push_back(token);
            }
            grid_size_x = std::stoi(tokens[0]);
            grid_size_y = std::stoi(tokens[1]);

            // Allocate the vectors
            velocity_x.reserve(grid_size_x.value());
            for (int i = 0; i < grid_size_x; i++) {
                velocity_x[i].reserve(grid_size_y.value());
            }
            velocity_y.reserve(grid_size_x.value());
            for (int i = 0; i < grid_size_x; i++) {
                velocity_y[i].reserve(grid_size_y.value());
            }
            pressure.reserve(grid_size_x.value());
            for (int i = 0; i < grid_size_x; i++) {
                pressure[i].reserve(grid_size_y.value());
            }
            dye.reserve(grid_size_x.value());
            for (int i = 0; i < grid_size_x; i++) {
                dye[i].reserve(grid_size_y.value());
            }
        } else if (line.find("density") != std::string::npos) {
            getline(file, line);
            density = std::stod(line);
        } else if (line.find("viscosity") != std::string::npos) {
            getline(file, line);
            viscosity = std::stod(line);
        } else if (line.find("execution_time") != std::string::npos) {
            getline(file, line);
            execution_time = std::stod(line);
        } else if (line.find("dt") != std::string::npos) {
            getline(file, line);
            dt = std::stod(line);
        } else if (line.find("normalization") != std::string::npos) {
            getline(file, line);
            std::stringstream ss(line);
            std::vector<std::string> tokens;
            while (ss.good()) {
                std::string token;
                getline(ss, token, ',');
                tokens.push_back(token);
            }
            EquationType equation_type = str_to_equation_type(tokens[0]);
            normalizations[equation_type] = std::stod(tokens[1]);
            if (equation_type == EquationType::PressureCorrection) {
                mass_normalization = std::stod(tokens[2]);
            }
        } else if (line.find("tolerance") != std::string::npos) {
            getline(file, line);
            std::stringstream ss(line);
            std::vector<std::string> tokens;
            while (ss.good()) {
                std::string token;
                getline(ss, token, ',');
                tokens.push_back(token);
            }
            EquationType equation_type = str_to_equation_type(tokens[0]);
            tolerances[equation_type] = std::stod(tokens[1]);
        } else if (line.find("velocity_u") != std::string::npos) {
            timesteps += 1;

            for (int i = 0; i < grid_size_x; i++) {
                for (int j = 0; j < grid_size_y; j++) {
                    getline(file, line);
                    std::stringstream ss(line);
                    std::vector<std::string> tokens;
                    while (ss.good()) {
                        std::string token;
                        getline(ss, token, ',');
                        tokens.push_back(token);
                    }

                    int x = std::stoi(tokens[0]);
                    int y = std::stoi(tokens[1]);
                    if (tokens[2] != "-") {
                        velocity_x[x][y] = std::stod(tokens[2]);
                    }
                }
            }
        } else if (line.find("velocity_v") != std::string::npos) {
            for (int i = 0; i < grid_size_x; i++) {
                for (int j = 0; j < grid_size_y; j++) {
                    getline(file, line);
                    std::stringstream ss(line);
                    std::vector<std::string> tokens;
                    while (ss.good()) {
                        std::string token;
                        getline(ss, token, ',');
                        tokens.push_back(token);
                    }

                    int x = std::stoi(tokens[0]);
                    int y = std::stoi(tokens[1]);
                    if (tokens[2] != "-") {
                        velocity_y[x][y] = std::stod(tokens[2]);
                    }
                }
            }
        } else if (line.find("pressure") != std::string::npos) {
            for (int i = 0; i < grid_size_x; i++) {
                for (int j = 0; j < grid_size_y; j++) {
                    getline(file, line);
                    std::stringstream ss(line);
                    std::vector<std::string> tokens;
                    while (ss.good()) {
                        std::string token;
                        getline(ss, token, ',');
                        tokens.push_back(token);
                    }

                    int x = std::stoi(tokens[0]);
                    int y = std::stoi(tokens[1]);
                    if (tokens[2] != "-") {
                        pressure[x][y] = std::stod(tokens[2]);
                    }
                }
            }
        } else if (line.find("dye") != std::string::npos) {
            for (int i = 0; i < grid_size_x; i++) {
                for (int j = 0; j < grid_size_y; j++) {
                    getline(file, line);
                    std::stringstream ss(line);
                    std::vector<std::string> tokens;
                    while (ss.good()) {
                        std::string token;
                        getline(ss, token, ',');
                        tokens.push_back(token);
                    }

                    int x = std::stoi(tokens[0]);
                    int y = std::stoi(tokens[1]);
                    if (tokens[2] != "-") {
                        dye[x][y] = std::stod(tokens[2]);
                    }
                }
            }
        }
    }
}
