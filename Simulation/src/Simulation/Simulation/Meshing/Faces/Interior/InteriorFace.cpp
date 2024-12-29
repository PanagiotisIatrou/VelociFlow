#include "InteriorFace.hpp"

InteriorFace::InteriorFace(const double dx, const double dy, const Orientation orientation) : Face(FaceType::Interior, orientation) {
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

double InteriorFace::get_viscosity() const {
    return m_viscosity;
}

void InteriorFace::update_density() {
    m_density = 0.5 * (m_node_neighbours[0]->get_density() + m_node_neighbours[1]->get_density());
}

double InteriorFace::get_density() const {
    return m_density;
}

double InteriorFace::get_dye() const {
    return m_dye;
}

void InteriorFace::set_dye(const double dye) {
    m_dye = dye;
}

void InteriorFace::update_dye() {
    m_dye = 0.5 * (m_node_neighbours[0]->get_field_value(Field::Dye) + m_node_neighbours[1]->get_field_value(Field::Dye));
}

double InteriorFace::get_pressure() const {
    return m_pressure;
}

void InteriorFace::update_pressure_correction() {
    m_pressure_correction = 0.5 * (m_node_neighbours[0]->get_field_value(Field::PressureCorrection) + m_node_neighbours[1]->get_field_value(Field::PressureCorrection));
}

double InteriorFace::get_pressure_correction() const {
    return m_pressure_correction;
}

void InteriorFace::update_pressure() {
    m_pressure = 0.5 * (m_node_neighbours[0]->get_field_value(Field::Pressure) + m_node_neighbours[1]->get_field_value(Field::Pressure));
}
