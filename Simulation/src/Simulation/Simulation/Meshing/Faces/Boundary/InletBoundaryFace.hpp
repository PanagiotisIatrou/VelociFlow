#pragma once

#include "BoundaryFace.hpp"

class InletBoundaryFace : public BoundaryFace {
private:
    double m_velocity_u;
    double m_velocity_v;
    double m_dye;

public:
    InletBoundaryFace(double velocity_u, double velocity_v, double dye, Orientation orientation);

    double get_velocity_u() const override;

    double get_velocity_v() const override;

    double get_dye() const override;

    double get_pressure_correction() const override;

    double get_pressure() const override;
};
