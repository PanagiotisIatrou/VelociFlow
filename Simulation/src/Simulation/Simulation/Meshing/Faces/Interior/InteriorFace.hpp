#pragma once

#include <array>

#include "../Face.hpp"
#include "../../Nodes/Node.hpp"

class Node;

class InteriorFace : public Face {
protected:
    double m_velocity = 0.0;
    double m_pressure = 0.0;
    double m_pressure_correction = 0.0;
    double m_dye = 0.0;
    double m_dx; // TODO: Remove this in the future (on non-cartesian grids)
    double m_dy; // TODO: Remove this in the future (on non-cartesian grids)
    std::array<Node *, 2> m_node_neighbours = {nullptr, nullptr};
public:
    InteriorFace(double dx, double dy, Orientation orientation);

    double get_velocity() const;

    virtual void update_velocity_distance_weighted() = 0;

    virtual void update_velocity_rhie_chow() = 0;

    void update_dye();

    void update_pressure_correction();

    void update_pressure();

    virtual void correct_velocity() = 0;

    double get_field_value(Field field) const override = 0;
};
