#pragma once

#include "BoundaryFace.hpp"

class FixedVelocityBoundaryFace : public BoundaryFace {
private:
    double m_velocity_u;
    double m_velocity_v;

public:
    FixedVelocityBoundaryFace(double velocity_u, double velocity_v);

    double get_velocity_u() const override;

    double get_velocity_v() const override;

    double get_pressure_correction() const override;

    double get_pressure() const override;
};
