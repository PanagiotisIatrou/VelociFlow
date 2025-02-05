#pragma once

#include "../EquationCoefficients.hpp"

class DyeEquationCoefficients : public EquationCoefficients {
public:
    DyeEquationCoefficients(Node *node, double relaxation, bool include_time);
};
