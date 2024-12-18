#pragma once

#include "../Coefficients.hpp"

class Node;

class PressureCorrectionCoefficients {
private:
    Node *m_node;

    Coefficients m_pressure_coefficients = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

public:
    PressureCorrectionCoefficients(Node *node);

    void calculate_pressure_coefficients();

    Coefficients get_pressure_coefficients() const;

    double get_pressure_coefficient(CoefficientType type) const;
};
