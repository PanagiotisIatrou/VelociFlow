#pragma once

#include "../EquationCoefficients.hpp"

class MomentumYCoefficients : public EquationCoefficients {
public:
    MomentumYCoefficients(Node *node, double relaxation, bool include_time);
};
