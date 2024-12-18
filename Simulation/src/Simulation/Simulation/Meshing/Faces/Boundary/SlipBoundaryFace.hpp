#pragma once

#include "BoundaryFace.hpp"

class SlipBoundaryFace : public BoundaryFace {
public:
    SlipBoundaryFace(Orientation orientation);

    double get_velocity_u() const override;

    double get_velocity_v() const override;

    double get_dye() const override;

    double get_pressure_correction() const override;

    double get_pressure() const override;
};
