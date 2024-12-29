#pragma once

enum class SimulationType { Steady, Unsteady };

enum class Field { VelocityX, VelocityY, Pressure, PressureCorrection, Dye };

enum class Direction { West = 0, East = 1, South = 2, North = 3, WestWest = 4, EastEast = 5, SouthSouth = 6, NorthNorth = 7 };
inline extern const int direction_start = 0;
inline extern const int direction_near_end = 4;
inline extern const int direction_all_end = 8;

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
    }
}

enum class VelocityComponent { U, V };

// Relaxation
inline extern const double velocity_u_relaxation = 0.5;
inline extern const double velocity_v_relaxation = 0.5;
inline extern const double pressure_relaxation = 0.2;
