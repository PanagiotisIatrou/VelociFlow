#pragma once

#include "BoundaryFace.hpp"

class FixedPressureBoundaryFace : public BoundaryFace {
private:
    double m_pressure;

public:
    FixedPressureBoundaryFace(double pressure, Orientation orientation);

    double get_velocity_u() const override;

    double get_velocity_v() const override;

    double get_pressure_correction() const override;

    double get_pressure() const override;
};
