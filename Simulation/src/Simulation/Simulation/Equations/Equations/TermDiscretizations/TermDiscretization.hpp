#pragma once

#include "../../Coefficients.hpp"

class Node;

class TermDiscretization {
protected:
    Node *m_node;

public:
    TermDiscretization(Node *node);

    virtual Coefficients get_effects() const = 0;
};
