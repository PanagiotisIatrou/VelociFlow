#pragma once

#include "InteriorFace.hpp"

enum class FaceXSide { West = 0, East = 1 };

class InteriorFaceX : public InteriorFace {
public:
    InteriorFaceX(double dx, double dy);

    void set_node_neighbour(Node *node, FaceXSide side);

    Node *get_node_neighbour(FaceXSide side) const;

    void update_velocity_distance_weighted() override;

    void update_velocity_rhie_chow() override;

    void correct_velocity() override;
};
