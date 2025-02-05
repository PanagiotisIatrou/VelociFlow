#pragma once

#include "../Equation.hpp"

class ConvectionDiffusionYEquation : public Equation {
public:
    ConvectionDiffusionYEquation(Mesh *mesh, Field variable_field, double relaxation_factor, ResidualType residual_type,
                         StoppingRule stopping_rule, NormType norm_type, double stopping_tolerance, bool include_time);
};
