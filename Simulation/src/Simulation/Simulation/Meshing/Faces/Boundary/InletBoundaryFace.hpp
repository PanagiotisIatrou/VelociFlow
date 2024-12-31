#pragma once

#include "BoundaryFace.hpp"

class InletBoundaryFace : public BoundaryFace {
private:
    double m_velocity_x;
    double m_velocity_y;
    double m_dye;

public:
    InletBoundaryFace(double velocity_x, double velocity_y, double dye, Orientation orientation);

    double get_field_value(Field field) const override;
};
