#pragma once

#include "BoundaryFace.hpp"

class MovingWallBoundaryFace : public BoundaryFace {
private:
    double m_velocity;

public:
    MovingWallBoundaryFace(double velocity, Orientation orientation);

    double get_velocity_u() const override;

    double get_velocity_v() const override;

    double get_dye() const override;

    double get_pressure_correction() const override;

    double get_pressure() const override;
};
