#include "BulkNodeOperations.hpp"

#include "../../Simulation.hpp"

BulkNodeOperations::BulkNodeOperations(Mesh *mesh) {
    m_mesh = mesh;
}

void BulkNodeOperations::correct_node_velocity_x() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->correct_velocity_x();
        }
    }
}

void BulkNodeOperations::correct_node_velocity_y() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->correct_velocity_y();
        }
    }
}

void BulkNodeOperations::update_node_previous_timestep_fields() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            // Update the previous timestep of all the fields
            for (int field = field_start; field < field_end; field++) {
                Field f = static_cast<Field>(field);
                node_P->set_previous_timestep_field_value(f, node_P->get_field_value(f));
            }
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

// Pressure correction

void BulkNodeOperations::reset_pressure_correction() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->set_field_value(Field::PressureCorrection, 0.0);
        }
    }
}

void BulkNodeOperations::set_dt(const double dt) const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->set_dt(dt);
        }
    }
}
