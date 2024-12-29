#include "InteriorFaceY.hpp"

#include "../../Nodes/Node.hpp"

InteriorFaceY::InteriorFaceY(const double dx, const double dy) : InteriorFace(dx, dy, Orientation::Vertical) {
}

void InteriorFaceY::set_node_neighbour(Node *node, const FaceYSide side) {
    m_node_neighbours[static_cast<int>(side)] = node;
}

Node *InteriorFaceY::get_node_neighbour(FaceYSide side) const {
    return m_node_neighbours[static_cast<int>(side)];
}

void InteriorFaceY::update_velocity_distance_weighted() {
    m_velocity = 0.5 * (get_node_neighbour(FaceYSide::South)->get_field_value(Field::VelocityY) + get_node_neighbour(FaceYSide::North)->get_field_value(Field::VelocityY));
}

void InteriorFaceY::update_velocity_rhie_chow() {
    const Node *node_P = get_node_neighbour(FaceYSide::South);
    const Node *node_N = get_node_neighbour(FaceYSide::North);

    const double velocity_y_P = node_P->get_field_value(Field::VelocityY);
    const double velocity_y_N = node_N->get_field_value(Field::VelocityY);

    const double pressure_P = node_P->get_field_value(Field::Pressure);
    const double pressure_N = node_N->get_field_value(Field::Pressure);

    const double momentum_y_a_P = node_P->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::V);
    const double momentum_y_a_N = node_N->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::V);

    const Face *face_s = node_P->get_neighbouring_face(Direction::South);
    const double pressure_s = face_s->get_pressure();

    const Face *face_n = node_P->get_neighbouring_face(Direction::North);
    const double pressure_n = face_n->get_pressure();

    const Face *face_nn = node_N->get_neighbouring_face(Direction::North);
    const double pressure_nn = face_nn->get_pressure();

    const double velocity_y_n = 0.5 * (velocity_y_P + velocity_y_N) + 0.5 * m_dt * m_dx * (
                                    (pressure_n - pressure_s) / momentum_y_a_P
                                    + (pressure_nn - pressure_n) / momentum_y_a_N
                                    - (1 / momentum_y_a_P + 1 / momentum_y_a_N) * (pressure_N - pressure_P)
                                );
    m_velocity = velocity_y_n;
}

void InteriorFaceY::correct_velocity() {
    const Node *node_P = get_node_neighbour(FaceYSide::South);
    const Node *node_N = get_node_neighbour(FaceYSide::North);

    const double momentum_y_a_P = node_P->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::V);
    const double momentum_y_a_N = node_N->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::V);
    const double correction = 0.5 * m_dt * m_dx
                      * (1 / momentum_y_a_P + 1 / momentum_y_a_N)
                      * (node_P->get_field_value(Field::PressureCorrection) - node_N->get_field_value(Field::PressureCorrection));

    m_velocity += correction;
}
