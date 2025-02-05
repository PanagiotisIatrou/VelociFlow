#pragma once

#include "../EquationCoefficients.hpp"

class DiffusionEquationCoefficients : public EquationCoefficients {
public:
    DiffusionEquationCoefficients(Node *node, Field variable_field, double relaxation, bool include_time);
};
