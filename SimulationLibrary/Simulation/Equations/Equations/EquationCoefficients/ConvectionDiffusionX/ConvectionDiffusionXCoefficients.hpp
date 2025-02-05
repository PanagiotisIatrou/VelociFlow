#pragma once

#include "../EquationCoefficients.hpp"

class ConvectionDiffusionXCoefficients : public EquationCoefficients {
public:
    ConvectionDiffusionXCoefficients(Node *node, double relaxation, bool include_time);
};
