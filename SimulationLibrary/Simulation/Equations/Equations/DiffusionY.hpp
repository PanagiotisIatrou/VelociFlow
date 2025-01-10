#pragma once

#include "../Equation.hpp"

class DiffusionY : public Equation {
public:
    DiffusionY(Mesh *mesh, Field variable_field, double relaxation_factor, ResidualType residual_type,
               StoppingRule stopping_rule, NormType norm_type, double stopping_tolerance, bool include_time);
};
