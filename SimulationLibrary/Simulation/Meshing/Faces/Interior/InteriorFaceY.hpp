#pragma once

#include "InteriorFace.hpp"

enum class FaceYSide { South = 0, North = 1 };

class InteriorFaceY : public InteriorFace {
public:
    InteriorFaceY(double dx, double dy);

    void set_node_neighbour(Node *node, FaceYSide side);

    Node *get_node_neighbour(FaceYSide side) const;

    void update_velocity_distance_weighted() override;

    void update_velocity_rhie_chow() override;

    void correct_velocity() override;

    double get_field_value(Field field) const override;
};
