#pragma once

#include "../EquationCoefficients.hpp"

class ConvectionDiffusionYCoefficients : public EquationCoefficients {
public:
    ConvectionDiffusionYCoefficients(Node *node, Field variable_field, double relaxation, bool include_time);
};
