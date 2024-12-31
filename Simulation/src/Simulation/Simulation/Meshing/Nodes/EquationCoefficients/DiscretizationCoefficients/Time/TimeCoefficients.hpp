#pragma once

#include "../Coefficients.hpp"

class Node;

class TimeCoefficients {
private:
    Node *m_node;

    bool m_include_density;

public:
    TimeCoefficients(Node *node, bool include_density);

    Coefficients get_time_effects(double previous_value) const;
};
