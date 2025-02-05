#include "MomentumYCoefficients.hpp"

#include "../../TermDiscretizations/Convection/Convection.hpp"
#include "../../TermDiscretizations/Diffusion/Diffusion.hpp"
#include "../../TermDiscretizations/Pressure/Pressure.hpp"
#include "../../TermDiscretizations/Time/Time.hpp"

MomentumYCoefficients::MomentumYCoefficients(Node *node, const double relaxation, const bool include_time)
    : EquationCoefficients(node, Field::VelocityY, relaxation) {
    // Diffusion
    std::unique_ptr<Diffusion> diffusion_equation =
        std::make_unique<Diffusion>(node, Field::VelocityY, DiffusionSchemes::CentralDifferencing);
    m_term_discretizations.push_back(std::move(diffusion_equation));

    // Convection
    std::unique_ptr<Convection> convection_equation =
        std::make_unique<Convection>(node, Field::VelocityY, ConvectionSchemes::QuickHayase, true);
    m_term_discretizations.push_back(std::move(convection_equation));

    // Pressure
    std::unique_ptr<Pressure> pressure_equation = std::make_unique<Pressure>(node, Component::Y);
    m_term_discretizations.push_back(std::move(pressure_equation));

    // Time
    if (include_time) {
        std::unique_ptr<Time> time_equation = std::make_unique<Time>(node, Field::VelocityY, true);
        m_term_discretizations.push_back(std::move(time_equation));
    }
}
