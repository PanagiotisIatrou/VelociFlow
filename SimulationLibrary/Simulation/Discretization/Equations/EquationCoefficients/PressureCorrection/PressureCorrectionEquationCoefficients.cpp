#include "PressureCorrectionEquationCoefficients.hpp"

#include "../../TermDiscretizations/SimplePressureCorrectionDivergence/SimplePressureCorrectionDivergence.hpp"

PressureCorrectionEquationCoefficients::PressureCorrectionEquationCoefficients(Node *node, const double relaxation)
    : EquationCoefficients(node, Field::PressureCorrection, relaxation) {
    // Pressure correction divergence
    std::unique_ptr<SimplePressureCorrectionDivergence> pressure_correction_divergence =
        std::make_unique<SimplePressureCorrectionDivergence>(node);
    m_term_discretizations.push_back(std::move(pressure_correction_divergence));
}
