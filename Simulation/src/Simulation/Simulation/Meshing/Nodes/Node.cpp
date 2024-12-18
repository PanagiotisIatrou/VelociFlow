#include "Node.hpp"

#include <memory>

#include "../../../common.hpp"

Node::Node(const double viscosity, const double density, const double dx, const double dy, const double velocity_u,
           const double velocity_v, const double pressure, const double dye) {
    m_viscosity = viscosity;
    m_density = density;
    m_dye = dye;
    m_dx = dx;
    m_dy = dy;
    m_velocity_u = velocity_u;
    m_velocity_v = velocity_v;
    m_pressure = pressure;

    m_momentum_coefficients = std::make_unique<MomentumCoefficients>(this);
    m_pressure_correction_coefficients = std::make_unique<PressureCorrectionCoefficients>(this);
    m_dye_coefficients = std::make_unique<DyeCoefficients>(this);

    m_previous_timestep_velocity_u = m_velocity_u;
    m_previous_timestep_velocity_v = m_velocity_v;
    m_previous_timestep_pressure = m_pressure;
}

void Node::set_velocity_u(const double velocity_u) {
    m_velocity_u = velocity_u;
}

double Node::get_velocity_u() const {
    return m_velocity_u;
}

void Node::set_previous_timestep_velocity_u(const double velocity) {
    m_previous_timestep_velocity_u = velocity;
}

double Node::get_previous_timestep_velocity_u() const {
    return m_previous_timestep_velocity_u;
}

void Node::set_velocity_v(const double velocity_v) {
    m_velocity_v = velocity_v;
}

double Node::get_velocity_v() const {
    return m_velocity_v;
}

void Node::set_previous_timestep_velocity_v(const double velocity) {
    m_previous_timestep_velocity_v = velocity;
}

double Node::get_previous_timestep_velocity_v() const {
    return m_previous_timestep_velocity_v;
}

void Node::set_pressure(const double pressure) {
    m_pressure = pressure;
}

double Node::get_pressure() const {
    return m_pressure;
}

void Node::set_previous_timestep_pressure(const double pressure) {
    m_previous_timestep_pressure = pressure;
}

double Node::get_previous_timestep_pressure() const {
    return m_previous_timestep_pressure;
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

double Node::get_viscosity() const {
    return m_viscosity;
}

double Node::get_density() const {
    return m_density;
}

double Node::get_dye() const {
    return m_dye;
}

void Node::set_dye(const double dye) {
    m_dye = dye;
}

void Node::set_previous_timestep_dye(const double dye) {
    m_previous_timestep_dye = dye;
}

double Node::get_previous_timestep_dye() const {
    return m_previous_timestep_dye;
}

double Node::get_pressure_correction() const {
    return m_pressure_correction;
}

void Node::set_pressure_correction(const double pressure_correction) {
    m_pressure_correction = pressure_correction;
}

void Node::apply_pressure_correction() {
    m_pressure += pressure_relaxation * m_pressure_correction;
}

void Node::correct_velocity_u() {
    const double pressure_correction_w = get_neighbouring_face(Direction::West)->get_pressure_correction();
    const double pressure_correction_e = get_neighbouring_face(Direction::East)->get_pressure_correction();
    const double momentum_u_a_P = get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
    const double correction = m_dt * m_dy * (pressure_correction_w - pressure_correction_e) / momentum_u_a_P;

    m_velocity_u += correction;
}

void Node::correct_velocity_v() {
    const double pressure_correction_s = get_neighbouring_face(Direction::South)->get_pressure_correction();
    const double pressure_correction_n = get_neighbouring_face(Direction::North)->get_pressure_correction();
    const double momentum_v_a_P = get_momentum_coefficient(CoefficientType::Center, VelocityComponent::V);
    const double correction = m_dt * m_dx * (pressure_correction_s - pressure_correction_n) / momentum_v_a_P;

    m_velocity_v += correction;
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
    m_momentum_coefficients->calculate_momentum_coefficients(velocity_component, simulation_type);
}

double Node::get_momentum_coefficient(const CoefficientType type, const VelocityComponent velocity_component) const {
    return m_momentum_coefficients->get_momentum_coefficient(type, velocity_component);
}

Coefficients Node::get_momentum_coefficients(const VelocityComponent velocity_component) const {
    return m_momentum_coefficients->get_momentum_coefficients(velocity_component);
}

void Node::calculate_pressure_coefficients() const {
    m_pressure_correction_coefficients->calculate_pressure_coefficients();
}

Coefficients Node::get_pressure_coefficients() const {
    return m_pressure_correction_coefficients->get_pressure_coefficients();
}

double Node::get_pressure_coefficient(const CoefficientType type) const {
    return m_pressure_correction_coefficients->get_pressure_coefficient(type);
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
