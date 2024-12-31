#include "Node.hpp"

#include <iostream>
#include <memory>

#include "../../../common.hpp"

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

    m_momentum_coefficients = std::make_unique<MomentumCoefficients>(this);
    m_pressure_correction_coefficients = std::make_unique<PressureCorrectionCoefficients>(this);
    m_dye_coefficients = std::make_unique<DyeCoefficients>(this);
    m_convection_diffusion_coefficients = std::make_unique<ConvectionDiffusionCoefficients>(this);

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

double Node::get_previous_timestep_variable_value(Variable variable) const {
    switch (variable) {
        case Variable::VelocityX: {
            return m_previous_timestep_velocity_x;
        }
        case Variable::VelocityY: {
            return m_previous_timestep_velocity_y;
        }
        case Variable::Pressure: {
            return m_previous_timestep_pressure;
        }
        case Variable::Dye: {
            return m_previous_timestep_dye;
        }
        default: {
            std::cerr << "Invalid variable type" << std::endl;
            exit(1);
        }
    }
}

double Node::set_previous_timestep_variable_value(Variable variable, double value) {
    switch (variable) {
        case Variable::VelocityX: {
            m_previous_timestep_velocity_x = value;
            break;
        }
        case Variable::VelocityY: {
            m_previous_timestep_velocity_y = value;
            break;
        }
        case Variable::Pressure: {
            m_previous_timestep_pressure = value;
            break;
        }
        case Variable::Dye: {
            m_previous_timestep_dye = value;
            break;
        }
        default: {
            std::cerr << "Invalid variable type" << std::endl;
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
    const double momentum_x_a_P = get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
    const double correction = m_dt * m_dy * (pressure_correction_w - pressure_correction_e) / momentum_x_a_P;

    m_velocity_x += correction;
}

void Node::correct_velocity_y() {
    const double pressure_correction_s = get_neighbouring_face(Direction::South)->get_field_value(Field::PressureCorrection);
    const double pressure_correction_n = get_neighbouring_face(Direction::North)->get_field_value(Field::PressureCorrection);
    const double momentum_y_a_P = get_momentum_coefficient(CoefficientType::Center, VelocityComponent::V);
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

void Node::calculate_momentum_coefficients(const VelocityComponent velocity_component,
                                           const SimulationType simulation_type) const {
    m_momentum_coefficients->calculate_coefficients(velocity_component, simulation_type);
}

double Node::get_momentum_coefficient(const CoefficientType type, const VelocityComponent velocity_component) const {
    return m_momentum_coefficients->get_coefficient(type, velocity_component);
}

Coefficients Node::get_momentum_coefficients(const VelocityComponent velocity_component) const {
    return m_momentum_coefficients->get_coefficients(velocity_component);
}

void Node::calculate_pressure_coefficients() const {
    m_pressure_correction_coefficients->calculate_coefficients();
}

Coefficients Node::get_pressure_coefficients() const {
    return m_pressure_correction_coefficients->get_coefficients();
}

double Node::get_pressure_coefficient(const CoefficientType type) const {
    return m_pressure_correction_coefficients->get_coefficient(type);
}

void Node::calculate_dye_coefficients(const SimulationType type) const {
    m_dye_coefficients->calculate_coefficients(type);
}

Coefficients Node::get_dye_coefficients() const {
    return m_dye_coefficients->get_coefficients();
}

double Node::get_dye_coefficient(const CoefficientType type) const {
    return m_dye_coefficients->get_coefficient(type);
}

void Node::calculate_convection_diffusion_coefficients(const SimulationType type, const VelocityComponent velocity_component) const {
    m_convection_diffusion_coefficients->calculate_coefficients(type, velocity_component);
}

Coefficients Node::get_convection_diffusion_coefficients(const VelocityComponent velocity_component) const {
    return m_convection_diffusion_coefficients->get_coefficients(velocity_component);
}

double Node::get_convection_diffusion_coefficient(CoefficientType type, VelocityComponent velocity_component) const {
    return m_convection_diffusion_coefficients->get_coefficient(type, velocity_component);
}
