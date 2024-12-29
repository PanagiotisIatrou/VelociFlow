#pragma once

#include <cassert>

#include "helper.hpp"

#include "../src/Simulation/common.hpp"
#include "../src/MeshExamples.hpp"
#include "../src/Simulation/Simulation/Meshing/Faces/Boundary/BoundaryFace.hpp"
#include "../src/Simulation/Simulation/Meshing/Faces/Interior/InteriorFaceX.hpp"
#include "../src/Simulation/Simulation/Meshing/Faces/Interior/InteriorFaceY.hpp"

inline void test_node_face_connections() {
    Mesh *mesh = create_test_mesh();

    int boundary_faces_x_count = 0;
    int boundary_faces_y_count = 0;
    for (int j = 0; j < mesh->get_size_y(); j++) {
        for (int i = 0; i < mesh->get_size_x(); i++) {
            Node *node_P = mesh->get_node(i, j);

            // Check if the node faces are connected to the node
            Node* node;

            // West node
            Face *face_w = node_P->get_neighbouring_face(Direction::West);
            if (face_w->get_face_type() == FaceType::Boundary) {
                node = static_cast<BoundaryFace *>(face_w)->get_node_neighbour();
                boundary_faces_x_count++;
            } else {
                node = static_cast<InteriorFaceX *>(face_w)->get_node_neighbour(FaceXSide::East);
            }
            assert(node == node_P);

            // East node
            Face *face_e = node_P->get_neighbouring_face(Direction::East);
            if (face_e->get_face_type() == FaceType::Boundary) {
                node = static_cast<BoundaryFace *>(face_e)->get_node_neighbour();
                boundary_faces_x_count++;
            } else {
                node = static_cast<InteriorFaceX *>(face_e)->get_node_neighbour(FaceXSide::West);
            }
            assert(node == node_P);

            // South node
            Face *face_s = node_P->get_neighbouring_face(Direction::South);
            if (face_s->get_face_type() == FaceType::Boundary) {
                node = static_cast<BoundaryFace *>(face_s)->get_node_neighbour();
                boundary_faces_y_count++;
            } else {
                node = static_cast<InteriorFaceY *>(face_s)->get_node_neighbour(FaceYSide::North);
            }
            assert(node == node_P);

            // South node
            Face *face_n = node_P->get_neighbouring_face(Direction::North);
            if (face_n->get_face_type() == FaceType::Boundary) {
                node = static_cast<BoundaryFace *>(face_n)->get_node_neighbour();
                boundary_faces_y_count++;
            } else {
                node = static_cast<InteriorFaceY *>(face_n)->get_node_neighbour(FaceYSide::South);
            }
            assert(node == node_P);

            // Check if the nodes are linked
            Node* node_W = node_P->get_neighbouring_node(Direction::West);
            if (node_W != nullptr) {
                assert(node_W->get_neighbouring_node(Direction::East) == node_P);
            }

            Node* node_E = node_P->get_neighbouring_node(Direction::East);
            if (node_E != nullptr) {
                assert(node_E->get_neighbouring_node(Direction::West) == node_P);
            }

            Node* node_S = node_P->get_neighbouring_node(Direction::South);
            if (node_S != nullptr) {
                assert(node_S->get_neighbouring_node(Direction::North) == node_P);
            }

            Node* node_N = node_P->get_neighbouring_node(Direction::North);
            if (node_N != nullptr) {
                assert(node_N->get_neighbouring_node(Direction::South) == node_P);
            }

            // Check if the 2 nodes share the same face
            if (node_W != nullptr) {
                assert(node_P->get_neighbouring_face(Direction::West) == node_W->get_neighbouring_face(Direction::East));
            }
            if (node_E != nullptr) {
                assert(node_P->get_neighbouring_face(Direction::East) == node_E->get_neighbouring_face(Direction::West));
            }
            if (node_S != nullptr) {
                assert(node_P->get_neighbouring_face(Direction::South) == node_S->get_neighbouring_face(Direction::North));
            }
            if (node_N != nullptr) {
                assert(node_P->get_neighbouring_face(Direction::North) == node_N->get_neighbouring_face(Direction::South));
            }
        }
    }

    assert(boundary_faces_x_count == 200 && boundary_faces_y_count == 200);
}

inline void test_rhie_chow() {
    Mesh *mesh = create_test_mesh();

    Node* node_P = mesh->get_node(0, 0);

    Node* node_E = mesh->get_node(1, 0);
    InteriorFaceX* face_e = static_cast<InteriorFaceX *>(node_P->get_neighbouring_face(Direction::East));

    Node* node_N = mesh->get_node(0, 1);
    InteriorFaceY* face_n = static_cast<InteriorFaceY *>(node_P->get_neighbouring_face(Direction::North));

    node_P->calculate_momentum_coefficients(VelocityComponent::U, SimulationType::Steady);
    node_P->calculate_momentum_coefficients(VelocityComponent::V, SimulationType::Steady);
    node_P->set_field_value(Field::VelocityX, 1.0);
    node_P->set_field_value(Field::VelocityY, 1.0);
    node_P->set_field_value(Field::Pressure, 0.0);

    node_E->calculate_momentum_coefficients(VelocityComponent::U, SimulationType::Steady);
    node_P->set_field_value(Field::VelocityX, 2.0);
    node_P->set_field_value(Field::Pressure, 0.0);

    node_N->calculate_momentum_coefficients(VelocityComponent::V, SimulationType::Steady);
    node_P->set_field_value(Field::VelocityY, 2.0);
    node_P->set_field_value(Field::Pressure, 0.0);

    face_e->update_velocity_rhie_chow();
    assert(face_e->get_velocity() == 1.5);

    face_n->update_velocity_rhie_chow();
    assert(face_n->get_velocity() == 1.5);
}

inline void run_all_tests() {
    test_node_face_connections();
    test_rhie_chow();
    std::cout << "[All tests ran successfully]" << std::endl;
}
