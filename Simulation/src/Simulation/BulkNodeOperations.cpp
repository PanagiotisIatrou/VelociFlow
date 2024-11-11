#include "BulkNodeOperations.h"

BulkNodeOperations::BulkNodeOperations(Mesh *mesh) {
    m_mesh = mesh;
}

void BulkNodeOperations::correct_node_velocity_u() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->correct_velocity_u();
        }
    }
}

void BulkNodeOperations::correct_node_velocity_v() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->correct_velocity_v();
        }
    }
}

void BulkNodeOperations::correct_node_pressure() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->apply_pressure_correction();
        }
    }
}

void BulkNodeOperations::calculate_momentum_coefficients(const VelocityComponent velocity_component) const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->calculate_momentum_coefficients(velocity_component);
        }
    }
}

void BulkNodeOperations::calculate_pressure_coefficients() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->calculate_pressure_coefficients();
        }
    }
}
