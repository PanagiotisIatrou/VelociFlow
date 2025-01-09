#pragma once

#include <memory>
#include <vector>

#include "../../Coefficients.hpp"
#include "../TermDiscretizations/TermDiscretization.hpp"

class EquationCoefficients {
protected:
    Node *m_node;

    Field m_variable_field;

    Coefficients m_coefficients;

    std::vector<std::unique_ptr<TermDiscretization>> m_term_discretizations;

    double m_relaxation;

public:
    EquationCoefficients(Node *node, Field variable_field, double relaxation);

    void calculate_coefficients();

    double get_coefficient(CoefficientType type) const;

    Coefficients get_coefficients() const;
};
