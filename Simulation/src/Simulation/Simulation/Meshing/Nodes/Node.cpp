#include "Node.hpp"

#include <iostream>
#include <memory>

#include "../../../common.hpp"
#include "../../Equations/Equations/EquationCoefficients/Dye/DyeCoefficients.hpp"
#include "../../Equations/Equations/EquationCoefficients/MomentumX/MomentumXCoefficients.hpp"
#include "../../Equations/Equations/EquationCoefficients/MomentumY/MomentumYCoefficients.hpp"
#include "../../Equations/Equations/EquationCoefficients/PressureCorrection/PressureCorrectionCoefficients.hpp"
#include "../../Equations/Equations/EquationCoefficients/ConvectionDiffusionX/ConvectionDiffusionXCoefficients.hpp"
#include "../../Equations/Equations/EquationCoefficients/ConvectionDiffusionY/ConvectionDiffusionYCoefficients.hpp"

Node::Node(const double viscosity, const double density, const double dx, const double dy, const double velocity_x,
           const double velocity_y, const double pressure, const double dye) {
    m_viscosity = viscosity;
    m_density = density;
    m_dye = dye;
    m_dx = dx;
    m_dy = dy;
    m_velocity_x = velocity_x;
    m_velocity_y = velocity_y;
    m_pressure = pressure;

    m_previous_timestep_velocity_x = m_velocity_x;
    m_previous_timestep_velocity_y = m_velocity_y;
    m_previous_timestep_pressure = m_pressure;
}

double Node::get_dx() const {
    return m_dx;
}

double Node::get_dy() const {
    return m_dy;
}

double Node::get_dt() const {
    return m_dt;
}

void Node::set_dt(const double dt) {
    m_dt = dt;
}

double Node::get_previous_timestep_field_value(const Field field) const {
    switch (field) {
        case Field::VelocityX: {
            return m_previous_timestep_velocity_x;
        }
        case Field::VelocityY: {
            return m_previous_timestep_velocity_y;
        }
        case Field::Pressure: {
            return m_previous_timestep_pressure;
        }
        case Field::Dye: {
            return m_previous_timestep_dye;
        }
        case Field::PressureCorrection: {
            return m_previous_timestep_pressure_correction;
        }
        case Field::Density: {
            return m_previous_timestep_density;
        }
        case Field::Viscosity: {
            return m_previous_timestep_viscosity;
        }
        default: {
            std::cerr << "Invalid field type" << std::endl;
            exit(1);
        }
    }
}

void Node::set_previous_timestep_field_value(const Field field, const double value) {
    switch (field) {
        case Field::VelocityX: {
            m_previous_timestep_velocity_x = value;
            break;
        }
        case Field::VelocityY: {
            m_previous_timestep_velocity_y = value;
            break;
        }
        case Field::Pressure: {
            m_previous_timestep_pressure = value;
            break;
        }
        case Field::Dye: {
            m_previous_timestep_dye = value;
            break;
        }
        case Field::PressureCorrection: {
            m_previous_timestep_pressure_correction = value;
            break;
        }
        case Field::Density: {
            m_previous_timestep_density = value;
            break;
        }
        case Field::Viscosity: {
            m_previous_timestep_viscosity = value;
            break;
        }
        default: {
            std::cerr << "Invalid field type" << std::endl;
            exit(1);
        }
    }
}

double Node::get_field_value(const Field field) const {
    switch (field) {
        case Field::VelocityX: {
            return m_velocity_x;
        }
        case Field::VelocityY: {
            return m_velocity_y;
        }
        case Field::Pressure: {
            return m_pressure;
        }
        case Field::PressureCorrection: {
            return m_pressure_correction;
        }
        case Field::Dye: {
            return m_dye;
        }
        case Field::Density: {
            return m_density;
        }
        case Field::Viscosity: {
            return m_viscosity;
        }
        default: {
            std::cerr << "Invalid field type" << std::endl;
            exit(1);
        }
    }
}

void Node::set_field_value(const Field field, const double value) {
    switch (field) {
        case Field::VelocityX: {
            m_velocity_x = value;
            break;
        }
        case Field::VelocityY: {
            m_velocity_y = value;
            break;
        }
        case Field::Pressure: {
            m_pressure = value;
            break;
        }
        case Field::PressureCorrection: {
            m_pressure_correction = value;
            break;
        }
        case Field::Dye: {
            m_dye = value;
            break;
        }
        case Field::Density: {
            m_density = value;
            break;
        }
        case Field::Viscosity: {
            m_viscosity = value;
            break;
        }
        default: {
            std::cerr << "Invalid field type" << std::endl;
            exit(1);
        }
    }
}

void Node::apply_pressure_correction() {
    m_pressure += pressure_relaxation * m_pressure_correction;
}

void Node::correct_velocity_x() {
    const double pressure_correction_w = get_neighbouring_face(Direction::West)->get_field_value(Field::PressureCorrection);
    const double pressure_correction_e = get_neighbouring_face(Direction::East)->get_field_value(Field::PressureCorrection);
    const double momentum_x_a_P = get_equation_coefficient(EquationType::MomentumX, CoefficientType::Center);
    const double correction = m_dt * m_dy * (pressure_correction_w - pressure_correction_e) / momentum_x_a_P;

    m_velocity_x += correction;
}

void Node::correct_velocity_y() {
    const double pressure_correction_s = get_neighbouring_face(Direction::South)->get_field_value(Field::PressureCorrection);
    const double pressure_correction_n = get_neighbouring_face(Direction::North)->get_field_value(Field::PressureCorrection);
    const double momentum_y_a_P = get_equation_coefficient(EquationType::MomentumY, CoefficientType::Center);
    const double correction = m_dt * m_dx * (pressure_correction_s - pressure_correction_n) / momentum_y_a_P;

    m_velocity_y += correction;
}

Face *Node::get_neighbouring_face(Direction direction) const {
    return m_neighbouring_faces[static_cast<int>(direction)];
}

void Node::set_neighbouring_face(Face *face, Direction direction) {
    m_neighbouring_faces[static_cast<int>(direction)] = face;
}

Node *Node::get_neighbouring_node(Direction direction) const {
    return m_neighbouring_nodes[static_cast<int>(direction)];
}

void Node::set_neighbouring_node(Node *node, Direction direction) {
    m_neighbouring_nodes[static_cast<int>(direction)] = node;
}

void Node::add_equation_coefficient(const EquationType equation_type, Field variable_field, const double relaxation, const bool include_time) {
    switch (equation_type) {
        case EquationType::MomentumX: {
            m_equation_coefficients[equation_type] = std::make_unique<MomentumXCoefficients>(this, variable_field, relaxation, include_time);
            break;
        }
        case EquationType::MomentumY: {
            m_equation_coefficients[equation_type] = std::make_unique<MomentumYCoefficients>(this, variable_field, relaxation, include_time);
            break;
        }
        case EquationType::PressureCorrection: {
            m_equation_coefficients[equation_type] = std::make_unique<PressureCorrectionCoefficients>(this, variable_field, relaxation);
            break;
        }
        case EquationType::Dye: {
            m_equation_coefficients[equation_type] = std::make_unique<DyeCoefficients>(this, variable_field, relaxation, include_time);
            break;
        }
        case EquationType::ConvectionDiffusionX: {
            m_equation_coefficients[equation_type] = std::make_unique<ConvectionDiffusionXCoefficients>(this, variable_field, relaxation, include_time);
            break;
        }
        case EquationType::ConvectionDiffusionY: {
            m_equation_coefficients[equation_type] = std::make_unique<ConvectionDiffusionYCoefficients>(this, variable_field, relaxation, include_time);
            break;
        }
        default: {
            std::cerr << "Invalid equation type" << std::endl;
            exit(1);
        }
    }
}

double Node::get_equation_coefficient(const EquationType equation_type, const CoefficientType coefficient_type) const {
    const auto equation_coefficients = m_equation_coefficients.find(equation_type);
    if (equation_coefficients == m_equation_coefficients.end()) {
        std::cerr << "Equation coefficients not found" << std::endl;
        exit(1);
    }

    return equation_coefficients->second->get_coefficient(coefficient_type);
}

Coefficients Node::get_equation_coefficients(const EquationType equation_type) const {
    const auto equation_coefficients = m_equation_coefficients.find(equation_type);
    if (equation_coefficients == m_equation_coefficients.end()) {
        std::cerr << "Equation coefficients not found" << std::endl;
        exit(1);
    }

    return equation_coefficients->second->get_coefficients();
}

void Node::calculate_equation_coefficients(const EquationType equation_type) {
    const auto equation_coefficients = m_equation_coefficients.find(equation_type);
    if (equation_coefficients == m_equation_coefficients.end()) {
        std::cerr << "Equation coefficients not found" << std::endl;
        exit(1);
    }

    equation_coefficients->second->calculate_coefficients();
}
