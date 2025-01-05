#pragma once

#include "../EquationCoefficients.hpp"

class MomentumYCoefficients : public EquationCoefficients {
public:
    MomentumYCoefficients(Node *node, Field variable_field, double relaxation, bool include_time);
};
