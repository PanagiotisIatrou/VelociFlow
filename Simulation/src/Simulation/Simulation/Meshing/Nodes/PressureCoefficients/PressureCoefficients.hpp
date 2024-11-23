#pragma once

#include <array>

class Node;

class PressureCoefficients {
private:
    Node *m_node;

    std::array<double, 6> m_pressure_coefficients = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

public:
    PressureCoefficients(Node *node);

    void calculate_pressure_coefficients();

    std::array<double, 6> get_pressure_coefficients() const;

    double get_pressure_coefficient(CoefficientType type) const;
};
