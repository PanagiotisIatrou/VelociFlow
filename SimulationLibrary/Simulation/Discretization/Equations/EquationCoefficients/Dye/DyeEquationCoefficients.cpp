#include "DyeEquationCoefficients.hpp"

#include "../../TermDiscretizations/Convection/Convection.hpp"
#include "../../TermDiscretizations/Diffusion/Diffusion.hpp"
#include "../../TermDiscretizations/Time/Time.hpp"

DyeEquationCoefficients::DyeEquationCoefficients(Node *node, const double relaxation, const bool include_time)
    : EquationCoefficients(node, Field::Dye, relaxation) {
    // Diffusion
    std::unique_ptr<Diffusion> diffusion_equation =
        std::make_unique<Diffusion>(node, Field::Dye, DiffusionSchemes::CentralDifferencing);
    m_term_discretizations.push_back(std::move(diffusion_equation));

    // Convection
    std::unique_ptr<Convection> convection_equation =
        std::make_unique<Convection>(node, Field::Dye, ConvectionSchemes::QuickHayase, false);
    m_term_discretizations.push_back(std::move(convection_equation));

    // Time
    if (include_time) {
        std::unique_ptr<Time> time_equation = std::make_unique<Time>(node, Field::Dye, false);
        m_term_discretizations.push_back(std::move(time_equation));
    }
}
