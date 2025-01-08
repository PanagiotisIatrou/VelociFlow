#pragma once

#include "../TermDiscretization.hpp"

class Time : public TermDiscretization {
private:
    Field m_field;

    bool m_include_density;

public:
    Time(Node *node, Field field, bool include_density);

    Coefficients get_effects() const override;
};
