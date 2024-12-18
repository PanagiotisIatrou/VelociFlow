#pragma once

#include "BoundaryFace.hpp"

class FreeBoundaryFace : public BoundaryFace {
public:
    FreeBoundaryFace(Orientation orientation);

    double get_velocity_u() const override;

    double get_velocity_v() const override;

    double get_dye() const override;

    double get_pressure_correction() const override;

    double get_pressure() const override;
};
