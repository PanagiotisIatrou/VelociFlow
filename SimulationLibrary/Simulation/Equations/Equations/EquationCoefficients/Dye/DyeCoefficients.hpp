#pragma once

#include "../EquationCoefficients.hpp"

class DyeCoefficients : public EquationCoefficients {
public:
    DyeCoefficients(Node *node, double relaxation, bool include_time);
};
