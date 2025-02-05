#pragma once

#include "../EquationCoefficients.hpp"

class PressureCorrectionEquationCoefficients : public EquationCoefficients {
public:
    PressureCorrectionEquationCoefficients(Node *node, double relaxation);
};
