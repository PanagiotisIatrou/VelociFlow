#pragma once
#include <iostream>

enum class SimulationType { Steady, Unsteady };

enum class Field {
    VelocityX = 0,
    VelocityY = 1,
    Pressure = 2,
    PressureCorrection = 3,
    Dye = 4
};
inline extern const int field_start = 0;
inline extern const int field_end = static_cast<int>(Field::Dye) + 1;

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

enum class Component {
    X = 0,
    Y = 1
};

enum class EquationType {
    MomentumX = 0,
    MomentumY = 1,
    PressureCorrection = 2,
    Dye = 3,
    ConvectionDiffusionX = 4,
    ConvectionDiffusionY = 5
};

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
