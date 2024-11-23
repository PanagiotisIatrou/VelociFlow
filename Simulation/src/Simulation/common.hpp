#pragma once

enum class SimulationType { Steady, Unsteady };

enum class Field { VelocityU, VelocityV, Pressure };

enum class Direction { West = 0, East = 1, South = 2, North = 3 };
inline extern const int direction_start = 0;
inline extern const int direction_end = 4;

enum class VelocityComponent { U, V };

// Relaxation
inline extern const double velocity_u_relaxation = 0.5;
inline extern const double velocity_v_relaxation = 0.5;
inline extern const double pressure_relaxation = 0.2;
