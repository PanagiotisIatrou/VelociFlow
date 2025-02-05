#pragma once

#include "../EquationCoefficients.hpp"

class ConvectionDiffusionXEquationCoefficients : public EquationCoefficients {
public:
    ConvectionDiffusionXEquationCoefficients(Node *node, double relaxation, bool include_time);
};
