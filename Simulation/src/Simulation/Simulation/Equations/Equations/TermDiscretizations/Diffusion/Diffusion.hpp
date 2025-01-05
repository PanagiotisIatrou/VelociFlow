#pragma once

#include "../TermDiscretization.hpp"

enum class DiffusionSchemes { CentralDifferencing };

class Diffusion : public TermDiscretization {
private:
    DiffusionSchemes m_diffusion_scheme;

    Field m_field;

    Coefficients get_central_differencing_effects(Direction direction) const;

public:
    Diffusion(Node *node, Field field, DiffusionSchemes diffusion_scheme);

    Coefficients get_effects() const override;
};
