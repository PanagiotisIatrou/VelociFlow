#pragma once

#include <limits>

#include "../Equation.hpp"

class PressureCorrection : public Equation {
private:
    double m_mass_imbalance = std::numeric_limits<double>::infinity();

    bool m_can_update_mass_imbalance_normalization_factor = true;

    double m_mass_imbalance_normalization_factor;

public:
    PressureCorrection(Mesh *mesh, Field variable_field, double relaxation_factor, ResidualType residual_type,
                       StoppingRule stopping_rule, NormType norm_type, double stopping_tolerance);

    void calculate_mass_imbalance();

    double get_mass_imbalance() const;

    void reset_mass_imbalance();

    double get_mass_imbalance_normalization_factor() const;

    void set_mass_imbalance_normalization_factor(double factor);
};
