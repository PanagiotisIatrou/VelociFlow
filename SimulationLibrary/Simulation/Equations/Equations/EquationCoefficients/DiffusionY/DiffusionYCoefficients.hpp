#pragma once

#include "../EquationCoefficients.hpp"

class DiffusionYCoefficients : public EquationCoefficients {
public:
    DiffusionYCoefficients(Node *node, Field variable_field, double relaxation, bool include_time);
};
