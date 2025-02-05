#pragma once

#include "../EquationCoefficients.hpp"

class MomentumXCoefficients : public EquationCoefficients {
public:
    MomentumXCoefficients(Node *node, double relaxation, bool include_time);
};
