#include "Node.hpp"

#include <iostream>

#include "../../../common.hpp"

Node::Node(const double density, const double viscosity, const double dx, const double dy) {
    m_viscosity = viscosity;
    m_density = density;
    m_dx = dx;
    m_dy = dy;
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

void Node::set_dt(const double dt) {
    m_dt = dt;
}

double Node::get_viscosity() const {
    return m_viscosity;
}

double Node::get_density() const {
    return m_density;
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
