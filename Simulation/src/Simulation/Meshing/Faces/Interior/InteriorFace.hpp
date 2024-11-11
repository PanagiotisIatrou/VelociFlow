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
    double m_dx; // TODO: Remove this in the future
    double m_dy; // TODO: Remove this in the future
    std::array<Node *, 2> m_node_neighbours = {nullptr, nullptr};
public:
    InteriorFace(double dx, double dy);

    double get_velocity() const;

    void set_velocity(double velocity);

    virtual void update_velocity_distance_weighted() = 0;

    virtual void update_velocity_rhie_chow() = 0;

    void update_viscosity();

    double get_pressure() const override;

    void update_pressure_correction();

    double get_pressure_correction() const override;

    void update_pressure();

    virtual void correct_velocity() = 0;
};
