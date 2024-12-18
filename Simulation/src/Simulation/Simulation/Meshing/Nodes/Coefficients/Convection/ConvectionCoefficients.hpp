#pragma once

#include "../Coefficients.hpp"

class Node;

enum class ConvectionSchemes { Upwind, CentralDifferencing, QuickHayase };

class ConvectionCoefficients {
private:
    Node *m_node;

    bool m_include_density;

    Coefficients get_upwind_convection_effects(Direction direction, VelocityComponent velocity_component) const;

    Coefficients get_central_differencing_convection_effects(Direction direction,
                                                             VelocityComponent velocity_component) const;

    Coefficients get_quick_hayase_convection_effects(Direction direction, VelocityComponent velocity_component) const;

public:
    ConvectionCoefficients(Node *node, bool include_density);

    Coefficients get_convection_effects(VelocityComponent velocity_component,
                                        ConvectionSchemes convection_scheme) const;
};
