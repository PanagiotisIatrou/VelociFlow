#pragma once

#include "BoundaryFace.hpp"

class FixedValueBoundaryFace : public BoundaryFace {
private:
    double m_phi;

public:
    FixedValueBoundaryFace(double phi, Orientation orientation);

    double get_field_value(Field field) const override;
};
