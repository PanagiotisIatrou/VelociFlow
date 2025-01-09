#pragma once

#include "../EquationCoefficients.hpp"

class ConvectionDiffusionXCoefficients : public EquationCoefficients {
   public:
    ConvectionDiffusionXCoefficients(Node *node, Field variable_field, double relaxation, bool include_time);
};
