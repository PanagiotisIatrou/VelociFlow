#pragma once

#include "../../Coefficients/Coefficients.hpp"

class Node;

class PressureCorrectionCoefficients {
private:
    Node *m_node;

    Coefficients m_coefficients = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

public:
    PressureCorrectionCoefficients(Node *node);

    void calculate_coefficients();

    Coefficients get_coefficients() const;

    double get_coefficient(CoefficientType type) const;
};
