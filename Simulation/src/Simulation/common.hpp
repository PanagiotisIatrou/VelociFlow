#pragma once

enum class SimulationType { Steady, Unsteady };

enum class Field { VelocityU, VelocityV, Pressure };

// Relaxation
inline extern const double velocity_u_relaxation = 0.5;
inline extern const double velocity_v_relaxation = 0.5;
inline extern const double pressure_relaxation = 0.2;
