#include "BulkNodeOperations.hpp"

#include "../../Simulation.hpp"

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

void BulkNodeOperations::update_node_previous_timestep_velocity_u() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->set_previous_timestep_velocity_x(node_P->get_velocity_x());
        }
    }
}

void BulkNodeOperations::update_node_previous_timestep_velocity_v() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->set_previous_timestep_velocity_y(node_P->get_velocity_y());
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

void BulkNodeOperations::update_node_previous_timestep_pressure() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->set_previous_timestep_pressure(node_P->get_pressure());
        }
    }
}

void BulkNodeOperations::update_node_previous_timestep_dye() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->set_previous_timestep_dye(node_P->get_dye());
        }
    }
}

void BulkNodeOperations::calculate_momentum_coefficients(const VelocityComponent velocity_component, const SimulationType simulation_type) const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->calculate_momentum_coefficients(velocity_component, simulation_type);
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

void BulkNodeOperations::calculate_dye_coefficients(const SimulationType simulation_type) const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->calculate_dye_coefficients(simulation_type);
        }
    }
}

void BulkNodeOperations::calculate_convection_diffusion_coefficients(const SimulationType simulation_type, const VelocityComponent velocity_component) const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->calculate_convection_diffusion_coefficients(simulation_type, velocity_component);
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
