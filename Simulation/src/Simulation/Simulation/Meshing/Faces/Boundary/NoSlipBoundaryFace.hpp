#pragma once

#include "BoundaryFace.hpp"

class NoSlipBoundaryFace : public BoundaryFace {
public:
    NoSlipBoundaryFace(Orientation orientation);

    double get_field_value(Field field) const override;
};
