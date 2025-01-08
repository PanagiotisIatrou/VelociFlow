#pragma once

#include "../TermDiscretization.hpp"

enum class ConvectionSchemes { Upwind, CentralDifferencing, QuickHayase };

class Convection : public TermDiscretization {
private:
    bool m_include_density;

    Field m_field;

    ConvectionSchemes m_convection_scheme;

    Coefficients get_upwind_effects(Direction direction) const;

    Coefficients get_central_differencing_effects(Direction direction) const;

    Coefficients get_quick_hayase_effects(Direction direction) const;

public:
    Convection(Node *node, Field field, ConvectionSchemes convection_scheme, bool include_density);

    Coefficients get_effects() const override;
};
