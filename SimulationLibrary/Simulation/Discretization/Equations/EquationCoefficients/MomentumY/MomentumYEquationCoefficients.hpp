#pragma once

#include "../EquationCoefficients.hpp"

class MomentumYEquationCoefficients : public EquationCoefficients {
public:
    MomentumYEquationCoefficients(Node *node, double relaxation, bool include_time);
};
