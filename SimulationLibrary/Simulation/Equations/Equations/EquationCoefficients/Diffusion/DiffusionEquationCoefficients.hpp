#pragma once

#include "../EquationCoefficients.hpp"

class DiffusionEquationCoefficients : public EquationCoefficients {
public:
    DiffusionEquationCoefficients(Node *node, double relaxation, bool include_time);
};
