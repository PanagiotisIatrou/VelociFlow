#pragma once

#include "../Coefficients.hpp"

class Node;

class PressureCoefficients {
private:
    Node *m_node;

public:
    PressureCoefficients(Node *node);

    Coefficients get_pressure_effects(VelocityComponent velocity_component) const;
};
