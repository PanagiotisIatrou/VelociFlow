#pragma once

#include "../EquationCoefficients.hpp"

class ConvectionDiffusionYEquationCoefficients : public EquationCoefficients {
public:
    ConvectionDiffusionYEquationCoefficients(Node *node, double relaxation, bool include_time);
};
