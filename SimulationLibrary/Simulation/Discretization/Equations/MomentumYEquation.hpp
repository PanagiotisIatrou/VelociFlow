#pragma once

#include "../Equation.hpp"

class MomentumYEquation : public Equation {
public:
    MomentumYEquation(Mesh *mesh, Field variable_field, double relaxation_factor, ResidualType residual_type,
              StoppingRule stopping_rule, NormType norm_type, double stopping_tolerance, bool include_time);
};
