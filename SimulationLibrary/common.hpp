#pragma once

#include <ncurses.h>

#include <iostream>

enum class Field { VelocityX = 0, VelocityY = 1, Pressure = 2, PressureCorrection = 3, Dye = 4, Phi = 5 };
inline extern const int field_start = 0;
inline extern const int field_end = static_cast<int>(Field::Phi) + 1;

enum class Direction {
    West = 0,
    East = 1,
    South = 2,
    North = 3,
    WestWest = 4,
    EastEast = 5,
    SouthSouth = 6,
    NorthNorth = 7
};
inline extern const int direction_start = 0;
inline extern const int direction_near_end = static_cast<int>(Direction::North) + 1;
inline extern const int direction_all_end = static_cast<int>(Direction::NorthNorth) + 1;

enum class Component { X = 0, Y = 1 };

enum class SimulationName {
    NavierStokesSteady = 0,
    NavierStokesUnsteady = 1,
    ConvectionDiffusionSteady = 2,
    ConvectionDiffusionUnsteady = 3,
    DiffusionSteady = 4,
    DiffusionUnsteady = 5,
    SingleConvectionDiffusionSteady = 6,
    SingleConvectionDiffusionUnsteady = 7
};

inline SimulationName str_to_simulation_name(const std::string simulation_name) {
    if (simulation_name == "navier_stokes_steady") {
        return SimulationName::NavierStokesSteady;
    } else if (simulation_name == "navier_stokes_unsteady") {
        return SimulationName::NavierStokesUnsteady;
    } else if (simulation_name == "convection_diffusion_steady") {
        return SimulationName::ConvectionDiffusionSteady;
    } else if (simulation_name == "convection_diffusion_unsteady") {
        return SimulationName::ConvectionDiffusionUnsteady;
    } else if (simulation_name == "diffusion_steady") {
        return SimulationName::DiffusionSteady;
    } else if (simulation_name == "diffusion_unsteady") {
        return SimulationName::DiffusionUnsteady;
    } else if (simulation_name == "single_convection_diffusion_steady") {
        return SimulationName::SingleConvectionDiffusionSteady;
    } else if (simulation_name == "single_convection_diffusion_unsteady") {
        return SimulationName::SingleConvectionDiffusionUnsteady;
    } else {
        std::cerr << "Simulation name not found" << std::endl;
        exit(1);
    }
}

inline std::string simulation_name_to_str(const SimulationName simulation_name) {
    switch (simulation_name) {
        case SimulationName::NavierStokesSteady: {
            return "navier_stokes_steady";
        }
        case SimulationName::NavierStokesUnsteady: {
            return "navier_stokes_unsteady";
        }
        case SimulationName::ConvectionDiffusionSteady: {
            return "convection_diffusion_steady";
        }
        case SimulationName::ConvectionDiffusionUnsteady: {
            return "convection_diffusion_unsteady";
        }
        case SimulationName::DiffusionSteady: {
            return "diffusion_steady";
        }
        case SimulationName::DiffusionUnsteady: {
            return "diffusion_unsteady";
        }
        case SimulationName::SingleConvectionDiffusionSteady: {
            return "single_convection_diffusion_steady";
        }
        case SimulationName::SingleConvectionDiffusionUnsteady: {
            return "single_convection_diffusion_unsteady";
        }
        default: {
            std::cerr << "Simulation name not found" << std::endl;
            exit(1);
        }
    }
}

enum class EquationType {
    MomentumX = 0,
    MomentumY = 1,
    PressureCorrection = 2,
    Dye = 3,
    ConvectionDiffusionX = 4,
    ConvectionDiffusionY = 5,
    DiffusionX = 6,
    SingleConvectionDiffusion = 7
};

inline std::string equation_type_to_str(const EquationType equation_type) {
    switch (equation_type) {
        case EquationType::MomentumX: {
            return "momentum_x";
        }
        case EquationType::MomentumY: {
            return "momentum_y";
        }
        case EquationType::PressureCorrection: {
            return "pressure_correction";
        }
        case EquationType::Dye: {
            return "dye";
        }
        case EquationType::ConvectionDiffusionX: {
            return "convection_diffusion_x";
        }
        case EquationType::ConvectionDiffusionY: {
            return "convection_diffusion_y";
        }
        case EquationType::DiffusionX: {
            return "diffusion_x";
        }
        case EquationType::SingleConvectionDiffusion: {
            return "single_convection_diffusion";
        }
        default: {
            std::cerr << "Invalid equation type" << std::endl;
            exit(1);
        }
    }
}

inline EquationType str_to_equation_type(const std::string equation_type) {
    if (equation_type == "momentum_x") {
        return EquationType::MomentumX;
    } else if (equation_type == "momentum_y") {
        return EquationType::MomentumY;
    } else if (equation_type == "pressure_correction") {
        return EquationType::PressureCorrection;
    } else if (equation_type == "dye") {
        return EquationType::Dye;
    } else if (equation_type == "convection_diffusion_x") {
        return EquationType::ConvectionDiffusionX;
    } else if (equation_type == "convection_diffusion_y") {
        return EquationType::ConvectionDiffusionY;
    } else if (equation_type == "diffusion_x") {
        return EquationType::DiffusionX;
    } else if (equation_type == "single_convection_diffusion") {
        return EquationType::SingleConvectionDiffusion;
    } else {
        std::cerr << "Equation type not found" << std::endl;
        exit(1);
    }
}

inline Direction extend_direction(const Direction direction) {
    switch (direction) {
        case Direction::West: {
            return Direction::WestWest;
        }
        case Direction::East: {
            return Direction::EastEast;
        }
        case Direction::South: {
            return Direction::SouthSouth;
        }
        case Direction::North: {
            return Direction::NorthNorth;
        }
        default: {
            std::cerr << "Invalid direction" << std::endl;
            exit(1);
        }
    }
}

inline Direction get_opposite_direction(const Direction direction) {
    switch (direction) {
        case Direction::West: {
            return Direction::East;
        }
        case Direction::East: {
            return Direction::West;
        }
        case Direction::South: {
            return Direction::North;
        }
        case Direction::North: {
            return Direction::South;
        }
        case Direction::WestWest: {
            return Direction::EastEast;
        }
        case Direction::EastEast: {
            return Direction::WestWest;
        }
        case Direction::SouthSouth: {
            return Direction::NorthNorth;
        }
        case Direction::NorthNorth: {
            return Direction::SouthSouth;
        }
        default: {
            std::cerr << "Invalid direction" << std::endl;
            exit(1);
        }
    }
}

// Relaxation
inline extern const double relaxation_velocity_x = 0.5;
inline extern const double relaxation_velocity_y = 0.5;
inline extern const double pressure_relaxation = 0.2;

// Ncurses

inline void start_ncurses() {
    initscr();
    noecho();
    nodelay(stdscr, TRUE);
}

inline void end_ncurses() {
    resetty();
    endwin();
}