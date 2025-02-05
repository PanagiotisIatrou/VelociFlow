#pragma once

#include "../EquationCoefficients.hpp"

class ConvectionDiffusionYCoefficients : public EquationCoefficients {
public:
    ConvectionDiffusionYCoefficients(Node *node, double relaxation, bool include_time);
};
