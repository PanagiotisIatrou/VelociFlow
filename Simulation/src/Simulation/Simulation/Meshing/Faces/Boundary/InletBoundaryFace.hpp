#pragma once

#include "BoundaryFace.hpp"

class InletBoundaryFace : public BoundaryFace {
private:
    double m_velocity_x;
    double m_velocity_y;
    double m_dye;

public:
    InletBoundaryFace(double velocity_x, double velocity_y, double dye, Orientation orientation);

    double get_velocity_x() const override;

    double get_velocity_y() const override;

    double get_dye() const override;

    double get_pressure_correction() const override;

    double get_pressure() const override;
};
