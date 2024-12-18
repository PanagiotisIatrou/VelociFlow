#pragma once

#include "../Coefficients.hpp"

class Node;

enum class DiffusionSchemes { CentralDifferencing };

class DiffusionCoefficients {
private:
    Node *m_node;

    Coefficients get_central_differencing_diffusion_effects(Direction direction,
                                                            VelocityComponent velocity_component) const;

public:
    DiffusionCoefficients(Node *node);

    Coefficients get_diffusion_effects(VelocityComponent velocity_component, DiffusionSchemes diffusion_scheme) const;
};
