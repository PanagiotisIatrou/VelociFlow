#include "InteriorFace.hpp"

InteriorFace::InteriorFace(const double dx, const double dy) : Face(FaceType::Interior) {
    m_dx = dx;
    m_dy = dy;
}

double InteriorFace::get_velocity() const {
    return m_velocity;
}

void InteriorFace::set_velocity(const double velocity) {
    m_velocity = velocity;
}

void InteriorFace::set_dt(const double dt) {
    m_dt = dt;
}

void InteriorFace::update_viscosity() {
    m_viscosity = 0.5 * (m_node_neighbours[0]->get_viscosity() + m_node_neighbours[1]->get_viscosity());
}

double InteriorFace::get_pressure() const {
    return m_pressure;
}

void InteriorFace::update_pressure_correction() {
    m_pressure_correction = 0.5 * (m_node_neighbours[0]->get_pressure_correction() + m_node_neighbours[1]->get_pressure_correction());
}

double InteriorFace::get_pressure_correction() const {
    return m_pressure_correction;
}

void InteriorFace::update_pressure() {
    m_pressure = 0.5 * (m_node_neighbours[0]->get_pressure() + m_node_neighbours[1]->get_pressure());
}
