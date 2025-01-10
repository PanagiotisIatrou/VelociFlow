#include "DiffusionXCoefficients.hpp"

#include "../../TermDiscretizations/Diffusion/Diffusion.hpp"
#include "../../TermDiscretizations/Time/Time.hpp"

DiffusionXCoefficients::DiffusionXCoefficients(Node *node, const Field variable_field, const double relaxation,
                                               const bool include_time)
    : EquationCoefficients(node, variable_field, relaxation) {
    // Diffusion
    std::unique_ptr<Diffusion> diffusion_equation =
        std::make_unique<Diffusion>(node, Field::VelocityX, DiffusionSchemes::CentralDifferencing);
    m_term_discretizations.push_back(std::move(diffusion_equation));

    // Time
    if (include_time) {
        std::unique_ptr<Time> time_equation = std::make_unique<Time>(node, Field::VelocityX, false);
        m_term_discretizations.push_back(std::move(time_equation));
    }
}
