#pragma once

#include "BoundaryFace.hpp"

class FixedPressureBoundaryFace : public BoundaryFace {
private:
    double m_pressure;

public:
    FixedPressureBoundaryFace(double pressure, Orientation orientation);

    double get_field_value(Field field) const override;
};
