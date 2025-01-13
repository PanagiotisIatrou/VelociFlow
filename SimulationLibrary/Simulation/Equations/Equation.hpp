#pragma once

#include <limits>

#include "../Meshing/Mesh.hpp"

enum class ResidualType {
    Unscaled,
    Scaled,
    Normalized,
};

enum class StoppingRule { Absolute, Relative };

enum class NormType { L1, L2, LInfinity };

class Equation {
   private:
    double m_imbalance;

    double m_imbalance_normalization_factor = 0.0;

    bool m_can_update_imbalance_normalization_factor = true;

   protected:
    Mesh *m_mesh;

    EquationType m_equation_type;

    Field m_variable_field;

    double m_relaxation_factor;

    int m_iterations_count = 0;

    ResidualType m_residual_type;

    StoppingRule m_stopping_rule;

    NormType m_norm_type;

    double m_stopping_tolerance;

    bool m_include_time;

    double m_lower_value_limit = -std::numeric_limits<double>::infinity();

    double m_upper_value_limit = std::numeric_limits<double>::infinity();

    double calculate_and_get_imbalance();

   public:
    Equation(Mesh *mesh, EquationType equation_type, Field variable_field, double relaxation_factor,
             ResidualType residual_type, StoppingRule stopping_rule, NormType norm_type, double stopping_tolerance,
             bool include_time);

    void populate_mesh() const;

    void calculate_coefficients() const;

    void solve();

    double get_imbalance() const;

    void calculate_imbalance();

    void reset_imbalance();

    double get_imbalance_normalization_factor() const;

    void set_imbalance_normalization_factor(double factor);

    double get_tolerance() const;

    void set_lower_value_limit(double limit);

    void set_upper_value_limit(double limit);

    void set_value_limits(double lower_limit, double upper_limit);

    static const int imbalance_normalization_iterations = 5;
};
