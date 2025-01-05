#pragma once

#include "../EquationCoefficients.hpp"

class PressureCorrectionCoefficients : public EquationCoefficients {
public:
    PressureCorrectionCoefficients(Node *node, Field variable_field, double relaxation);
};
