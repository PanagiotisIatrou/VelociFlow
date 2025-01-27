#pragma once

#include "../../Nodes/Node.hpp"
#include "../Face.hpp"

enum class BoundaryType { Inlet, NoSlip, MovingWall, Slip, FixedPressure, Free };

class BoundaryFace : public Face {
protected:
    BoundaryType m_boundary_type;
    Node *m_node_neighbour = nullptr;

public:
    BoundaryFace(BoundaryType boundary_type, Orientation orientation);

    BoundaryType get_boundary_type() const;

    void set_node_neighbour(Node *node);

    Node *get_node_neighbour() const;

    double get_normal_velocity() override;

    virtual double get_field_value(Field field) const = 0;
};
