#pragma once

#include "../Equation.hpp"

class ConvectionDiffusionXEquation : public Equation {
public:
    ConvectionDiffusionXEquation(Mesh *mesh, Field variable_field, double relaxation_factor, ResidualType residual_type,
                         StoppingRule stopping_rule, NormType norm_type, double stopping_tolerance, bool include_time);
};
