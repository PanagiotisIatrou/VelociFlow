#pragma once

#include "../EquationCoefficients.hpp"

class MomentumXEquationCoefficients : public EquationCoefficients {
public:
    MomentumXEquationCoefficients(Node *node, double relaxation, bool include_time);
};
