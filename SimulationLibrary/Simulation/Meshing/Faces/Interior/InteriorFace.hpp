#pragma once

#include <array>

#include "../../Nodes/Node.hpp"
#include "../Face.hpp"

enum class InteriorFaceSide { First = 0, Second = 1 };

class InteriorFace : public Face {
protected:
    double m_velocity = 0.0;
    double m_pressure = 0.0;
    double m_pressure_correction = 0.0;
    double m_dye = 0.0;
    double m_dx;  // TODO: Remove this in the future (on non-cartesian grids)
    double m_dy;  // TODO: Remove this in the future (on non-cartesian grids)
    std::array<Node *, 2> m_node_neighbours = {nullptr, nullptr};

public:
    InteriorFace(double dx, double dy, Orientation orientation);

    void set_node_neighbour(Node *node, InteriorFaceSide side);

    Node *get_node_neighbour(InteriorFaceSide side) const;

    void update_velocity_distance_weighted();

    void update_velocity_rhie_chow();

    void correct_velocity();

    double get_field_value(Field field) const;

    double get_velocity() const;

    double get_normal_velocity() override;

    void update_dye();

    void update_pressure_correction();

    void update_pressure();
};
