#pragma once

#include "BoundaryFace.hpp"

class MovingWallBoundaryFace : public BoundaryFace {
private:
    double m_velocity;

public:
    MovingWallBoundaryFace(double velocity, Orientation orientation);

    double get_field_value(Field field) const override;
};
