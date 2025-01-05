#pragma once

#include "../EquationCoefficients.hpp"

class MomentumXCoefficients : public EquationCoefficients {
public:
    MomentumXCoefficients(Node *node, Field variable_field, double relaxation, bool include_time);
};
