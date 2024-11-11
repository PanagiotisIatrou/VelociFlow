#include "InteriorFaceX.hpp"

#include "../../../common.hpp"
#include "../../Nodes/Node.hpp"

InteriorFaceX::InteriorFaceX(const double dx, const double dy) : InteriorFace(dx, dy) {
}

void InteriorFaceX::set_node_neighbour(Node *node, const FaceXSide side) {
    m_node_neighbours[static_cast<int>(side)] = node;
}

Node *InteriorFaceX::get_node_neighbour(FaceXSide side) const {
    return m_node_neighbours[static_cast<int>(side)];
}

void InteriorFaceX::update_velocity_distance_weighted() {
    m_velocity = 0.5 * (get_node_neighbour(FaceXSide::West)->get_velocity_u() + get_node_neighbour(FaceXSide::East)->get_velocity_u());
}

void InteriorFaceX::update_velocity_rhie_chow() {
    const Node *node_P = get_node_neighbour(FaceXSide::West);
    const Node *node_E = get_node_neighbour(FaceXSide::East);

    const double velocity_u_P = node_P->get_velocity_u();
    const double velocity_u_E = node_E->get_velocity_u();

    const double pressure_P = node_P->get_pressure();
    const double pressure_E = node_E->get_pressure();

    const double momentum_u_a_P = node_P->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
    const double momentum_u_a_E = node_E->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);

    const Face *face_w = node_P->get_neighbouring_face(Direction::West);
    const double pressure_w = face_w->get_pressure();

    const Face *face_e = node_P->get_neighbouring_face(Direction::East);
    const double pressure_e = face_e->get_pressure();

    const Face *face_ee = node_E->get_neighbouring_face(Direction::East);
    const double pressure_ee = face_ee->get_pressure();

    const double velocity_u_e = 0.5 * (velocity_u_P + velocity_u_E) + 0.5 * m_dy * (
                                    (pressure_e - pressure_w) / momentum_u_a_P
                                    + (pressure_ee - pressure_e) / momentum_u_a_E
                                    - (1 / momentum_u_a_P + 1 / momentum_u_a_E) * (pressure_E - pressure_P)
                                );
    m_velocity = velocity_u_e;
}

void InteriorFaceX::correct_velocity() {
    const Node *node_P = get_node_neighbour(FaceXSide::West);
    const Node *node_E = get_node_neighbour(FaceXSide::East);

    const double momentum_u_a_P = node_P->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
    const double momentum_u_a_E = node_E->get_momentum_coefficient(CoefficientType::Center, VelocityComponent::U);
    const double correction = 0.5 * m_dy
                      * (1 / momentum_u_a_P + 1 / momentum_u_a_E)
                      * (node_P->get_pressure_correction() - node_E->get_pressure_correction());

    m_velocity += correction;
}
