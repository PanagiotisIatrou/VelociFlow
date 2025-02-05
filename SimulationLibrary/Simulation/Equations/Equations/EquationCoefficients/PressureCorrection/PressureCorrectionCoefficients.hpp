#pragma once

#include "../EquationCoefficients.hpp"

class PressureCorrectionCoefficients : public EquationCoefficients {
public:
    PressureCorrectionCoefficients(Node *node, double relaxation);
};
