#pragma once

#include "../Face.hpp"

class Node;

class BoundaryFace : public Face {
protected:
    Node *m_node_neighbour = nullptr;

public:
    BoundaryFace(double viscosity);

    void set_node_neighbour(Node *node);

    Node *get_node_neighbour() const;

    virtual double get_velocity_u() const = 0;

    virtual double get_velocity_v() const = 0;

    double get_pressure_correction() const override = 0;

    double get_pressure() const override = 0;
};
