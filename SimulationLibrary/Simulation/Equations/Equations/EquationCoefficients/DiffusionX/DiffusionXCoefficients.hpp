#pragma once

#include "../EquationCoefficients.hpp"

class DiffusionXCoefficients : public EquationCoefficients {
public:
    DiffusionXCoefficients(Node *node, Field variable_field, double relaxation, bool include_time);
};
