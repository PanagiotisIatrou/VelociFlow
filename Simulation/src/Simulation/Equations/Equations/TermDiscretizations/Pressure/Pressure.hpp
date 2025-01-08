#pragma once

#include "../TermDiscretization.hpp"

class Pressure : public TermDiscretization {
private:
    Component m_component;

public:
    Pressure(Node *node, Component component);

    Coefficients get_effects() const override;
};
