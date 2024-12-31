#include "InteriorFaceX.hpp"

#include <iostream>

#include "../../Nodes/Node.hpp"

InteriorFaceX::InteriorFaceX(const double dx, const double dy) : InteriorFace(dx, dy, Orientation::Horizontal) {
}

void InteriorFaceX::set_node_neighbour(Node *node, const FaceXSide side) {
    m_node_neighbours[static_cast<int>(side)] = node;
}

Node *InteriorFaceX::get_node_neighbour(FaceXSide side) const {
    return m_node_neighbours[static_cast<int>(side)];
}

void InteriorFaceX::update_velocity_distance_weighted() {
    m_velocity = 0.5 * (get_node_neighbour(FaceXSide::West)->get_field_value(Field::VelocityX) + get_node_neighbour(FaceXSide::East)->get_field_value(Field::VelocityX));
}

void InteriorFaceX::update_velocity_rhie_chow() {
    const Node *node_P = get_node_neighbour(FaceXSide::West);
    const Node *node_E = get_node_neighbour(FaceXSide::East);

    const double velocity_x_P = node_P->get_field_value(Field::VelocityX);
    const double velocity_x_E = node_E->get_field_value(Field::VelocityX);

    const double pressure_P = node_P->get_field_value(Field::Pressure);
    const double pressure_E = node_E->get_field_value(Field::Pressure);

    const double momentum_x_a_P = node_P->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
    const double momentum_x_a_E = node_E->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);

    const Face *face_w = node_P->get_neighbouring_face(Direction::West);
    const double pressure_w = face_w->get_field_value(Field::Pressure);

    const Face *face_e = node_P->get_neighbouring_face(Direction::East);
    const double pressure_e = face_e->get_field_value(Field::Pressure);

    const Face *face_ee = node_E->get_neighbouring_face(Direction::East);
    const double pressure_ee = face_ee->get_field_value(Field::Pressure);

    const double velocity_x_e = 0.5 * (velocity_x_P + velocity_x_E) + 0.5 * m_dt * m_dy * (
                                    (pressure_e - pressure_w) / momentum_x_a_P
                                    + (pressure_ee - pressure_e) / momentum_x_a_E
                                    - (1 / momentum_x_a_P + 1 / momentum_x_a_E) * (pressure_E - pressure_P)
                                );
    m_velocity = velocity_x_e;
}

void InteriorFaceX::correct_velocity() {
    const Node *node_P = get_node_neighbour(FaceXSide::West);
    const Node *node_E = get_node_neighbour(FaceXSide::East);

    const double momentum_x_a_P = node_P->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
    const double momentum_x_a_E = node_E->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
    const double correction = 0.5 * m_dt * m_dy
                      * (1 / momentum_x_a_P + 1 / momentum_x_a_E)
                      * (node_P->get_field_value(Field::PressureCorrection) - node_E->get_field_value(Field::PressureCorrection));

    m_velocity += correction;
}

double InteriorFaceX::get_field_value(const Field field) const {
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
        default: {
            std::cerr << "Field not recognised" << std::endl;
            exit(1);
        }
    }
}
