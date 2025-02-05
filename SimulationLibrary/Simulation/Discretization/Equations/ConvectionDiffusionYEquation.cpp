#include "ConvectionDiffusionYEquation.hpp"

ConvectionDiffusionYEquation::ConvectionDiffusionYEquation(Mesh *mesh, const Field variable_field, const double relaxation_factor,
                                           const ResidualType residual_type, const StoppingRule stopping_rule,
                                           const NormType norm_type, const double stopping_tolerance,
                                           const bool include_time)
    : Equation(mesh, EquationType::ConvectionDiffusionY, variable_field, relaxation_factor, residual_type,
               stopping_rule, norm_type, stopping_tolerance, include_time) {
}