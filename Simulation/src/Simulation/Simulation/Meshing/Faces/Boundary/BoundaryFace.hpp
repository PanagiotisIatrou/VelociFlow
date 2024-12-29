#pragma once

#include "../Face.hpp"
#include "../../Nodes/Node.hpp"

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

    virtual double get_velocity_x() const = 0;

    virtual double get_velocity_y() const = 0;

    double get_pressure_correction() const override = 0;

    double get_pressure() const override = 0;

    double get_dye() const override = 0;

    double get_viscosity() const override;

    double get_density() const override;
};
