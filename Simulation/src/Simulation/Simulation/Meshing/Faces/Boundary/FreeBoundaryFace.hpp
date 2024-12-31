#pragma once

#include "BoundaryFace.hpp"

class FreeBoundaryFace : public BoundaryFace {
public:
    FreeBoundaryFace(Orientation orientation);

    double get_field_value(Field field) const override;
};
