#pragma once

#include "BoundaryFace.hpp"

class SlipBoundaryFace : public BoundaryFace {
public:
    SlipBoundaryFace(Orientation orientation);

    double get_field_value(Field field) const override;
};
