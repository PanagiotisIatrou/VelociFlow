#include "InteriorFace.hpp"

InteriorFace::InteriorFace(const double dx, const double dy, const Orientation orientation) : Face(FaceType::Interior, orientation) {
    m_dx = dx;
    m_dy = dy;
}

double InteriorFace::get_velocity() const {
    return m_velocity;
}

void InteriorFace::set_dt(const double dt) {
    m_dt = dt;
}

void InteriorFace::update_viscosity() {
    m_viscosity = 0.5 * (m_node_neighbours[0]->get_field_value(Field::Viscosity) + m_node_neighbours[1]->get_field_value(Field::Viscosity));
}

double InteriorFace::get_viscosity() const {
    return m_viscosity;
}

void InteriorFace::update_density() {
    m_density = 0.5 * (m_node_neighbours[0]->get_field_value(Field::Density) + m_node_neighbours[1]->get_field_value(Field::Density));
}

double InteriorFace::get_density() const {
    return m_density;
}

void InteriorFace::update_dye() {
    m_dye = 0.5 * (m_node_neighbours[0]->get_field_value(Field::Dye) + m_node_neighbours[1]->get_field_value(Field::Dye));
}

void InteriorFace::update_pressure_correction() {
    m_pressure_correction = 0.5 * (m_node_neighbours[0]->get_field_value(Field::PressureCorrection) + m_node_neighbours[1]->get_field_value(Field::PressureCorrection));
}

void InteriorFace::update_pressure() {
    m_pressure = 0.5 * (m_node_neighbours[0]->get_field_value(Field::Pressure) + m_node_neighbours[1]->get_field_value(Field::Pressure));
}
