#pragma once

#include "../EquationCoefficients.hpp"

class SingleConvectionDiffusionEquationCoefficients : public EquationCoefficients {
public:
    SingleConvectionDiffusionEquationCoefficients(Node *node, double relaxation, bool include_time);
};
