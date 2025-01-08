#pragma once

#include "../EquationCoefficients.hpp"

class DyeCoefficients : public EquationCoefficients {
public:
    DyeCoefficients(Node *node, Field variable_field, double relaxation, bool include_time);
};
