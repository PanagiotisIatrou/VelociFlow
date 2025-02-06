#include "InteriorFace.hpp"

InteriorFace::InteriorFace(const double dx, const double dy, const Orientation orientation)
    : Face(FaceType::Interior, orientation) {
    m_dx = dx;
    m_dy = dy;
}

void InteriorFace::set_node_neighbour(Node *node, const InteriorFaceSide side) {
    m_node_neighbours[static_cast<int>(side)] = node;
}

Node *InteriorFace::get_node_neighbour(InteriorFaceSide side) const {
    return m_node_neighbours[static_cast<int>(side)];
}

void InteriorFace::update_velocity_distance_weighted() {
    const Field field = m_orientation == Orientation::Horizontal ? Field::VelocityX : Field::VelocityY;
    m_velocity = 0.5 * (m_node_neighbours[0]->get_field_value(field) + m_node_neighbours[1]->get_field_value(field));
}

void InteriorFace::update_velocity_rhie_chow() {
    const Node *node_P = get_node_neighbour(InteriorFaceSide::First);
    const Node *node_second = get_node_neighbour(InteriorFaceSide::Second);

    const Field field = m_orientation == Orientation::Horizontal ? Field::VelocityX : Field::VelocityY;
    const double velocity_P = node_P->get_field_value(field);
    const double velocity_second = node_second->get_field_value(field);

    const double pressure_P = node_P->get_field_value(Field::Pressure);
    const double pressure_second = node_second->get_field_value(Field::Pressure);

    const EquationType momentum_equation_type =
        m_orientation == Orientation::Horizontal ? EquationType::MomentumX : EquationType::MomentumY;
    const double momentum_a_P = node_P->get_equation_coefficient(momentum_equation_type, CoefficientType::Center);
    const double momentum_a_second =
        node_second->get_equation_coefficient(momentum_equation_type, CoefficientType::Center);

    const Direction direction_first = m_orientation == Orientation::Horizontal ? Direction::West : Direction::South;
    const Direction direction_second = m_orientation == Orientation::Horizontal ? Direction::East : Direction::North;

    const Face *face_first = node_P->get_neighbouring_face(direction_first);
    const double pressure_face_first = face_first->get_field_value(Field::Pressure);

    const Face *face_second = node_P->get_neighbouring_face(direction_second);
    const double pressure_face_second = face_second->get_field_value(Field::Pressure);

    const Face *face_second_second = node_second->get_neighbouring_face(direction_second);
    const double pressure_face_second_second = face_second_second->get_field_value(Field::Pressure);

    const double area = m_orientation == Orientation::Horizontal ? m_dy : m_dx;

    const double velocity_face_second =
        0.5 * (velocity_P + velocity_second) +
        0.5 * m_dt * area *
            ((pressure_face_second - pressure_face_first) / momentum_a_P +
             (pressure_face_second_second - pressure_face_second) / momentum_a_second -
             (1 / momentum_a_P + 1 / momentum_a_second) * (pressure_second - pressure_P));
    m_velocity = velocity_face_second;
}

void InteriorFace::correct_velocity() {
    const Node *node_P = get_node_neighbour(InteriorFaceSide::First);
    const Node *node_second = get_node_neighbour(InteriorFaceSide::Second);

    const EquationType momentum_equation_type =
        m_orientation == Orientation::Horizontal ? EquationType::MomentumX : EquationType::MomentumY;
    const double momentum_a_P = node_P->get_equation_coefficient(momentum_equation_type, CoefficientType::Center);
    const double momentum_a_second =
        node_second->get_equation_coefficient(momentum_equation_type, CoefficientType::Center);
    const double area = m_orientation == Orientation::Horizontal ? m_dy : m_dx;
    const double correction =
        0.5 * m_dt * area * (1 / momentum_a_P + 1 / momentum_a_second) *
        (node_P->get_field_value(Field::PressureCorrection) - node_second->get_field_value(Field::PressureCorrection));

    m_velocity += correction;
}

double InteriorFace::get_field_value(const Field field) const {
    switch (field) {
        case Field::VelocityX: {
            return m_velocity;
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
        case Field::Phi: {
            return m_phi;
        }
        default: {
            std::cerr << "Field not recognised" << std::endl;
            exit(1);
        }
    }
}

double InteriorFace::get_velocity() const {
    return m_velocity;
}

double InteriorFace::get_normal_velocity() {
    return m_velocity;
}

void InteriorFace::update_dye() {
    m_dye =
        0.5 * (m_node_neighbours[0]->get_field_value(Field::Dye) + m_node_neighbours[1]->get_field_value(Field::Dye));
}

void InteriorFace::update_phi() {
    m_phi =
        0.5 * (m_node_neighbours[0]->get_field_value(Field::Phi) + m_node_neighbours[1]->get_field_value(Field::Phi));
}

void InteriorFace::update_pressure_correction() {
    m_pressure_correction = 0.5 * (m_node_neighbours[0]->get_field_value(Field::PressureCorrection) +
                                   m_node_neighbours[1]->get_field_value(Field::PressureCorrection));
}

void InteriorFace::update_pressure() {
    m_pressure = 0.5 * (m_node_neighbours[0]->get_field_value(Field::Pressure) +
                        m_node_neighbours[1]->get_field_value(Field::Pressure));
}
