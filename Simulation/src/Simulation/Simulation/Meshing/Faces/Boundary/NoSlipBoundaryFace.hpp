#pragma once

#include "BoundaryFace.hpp"

class NoSlipBoundaryFace : public BoundaryFace {
public:
    NoSlipBoundaryFace(Orientation orientation);

    double get_velocity_x() const override;

    double get_velocity_y() const override;

    double get_dye() const override;

    double get_pressure_correction() const override;

    double get_pressure() const override;
};
