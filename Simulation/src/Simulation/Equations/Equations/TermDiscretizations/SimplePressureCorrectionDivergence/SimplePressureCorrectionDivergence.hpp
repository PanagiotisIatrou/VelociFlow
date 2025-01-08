#pragma once

#include "../TermDiscretization.hpp"

class SimplePressureCorrectionDivergence : public TermDiscretization {
public:
    SimplePressureCorrectionDivergence(Node *node);

    Coefficients get_effects() const override;
};
