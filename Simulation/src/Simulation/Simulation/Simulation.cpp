#include "Simulation.hpp"

#include <cmath>
#include <iostream>

#include "Meshing/Faces/Boundary/BoundaryFace.hpp"
#include "Meshing/Faces/Interior/InteriorFace.hpp"

Simulation::Simulation(Mesh *mesh, const double velocity_u_tolerance, const double velocity_v_tolerance,
                       const double pressure_tolerance, const std::string output_file, const VerboseType verbose_type) {
    m_mesh = mesh;
    m_bulk_node_operations = std::make_unique<BulkNodeOperations>(mesh);
    m_bulk_face_operations = std::make_unique<BulkFaceOperations>(mesh);
    m_timer = std::make_unique<Timer>();
    m_saver = std::make_unique<Saver>(mesh, output_file);
    m_verbose_type = verbose_type;

    m_velocity_u_tolerance = velocity_u_tolerance;
    m_velocity_v_tolerance = velocity_v_tolerance;
    m_pressure_tolerance = pressure_tolerance;
}

double Simulation::get_time_taken() const {
    return m_time_taken;
}

void Simulation::calculate_active_cells_count() {
    m_active_cells_count = 0;
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P != nullptr) {
                m_active_cells_count++;
            }
        }
    }
}

void Simulation::calculate_momentum_x_imbalance() {
    m_momentum_x_error = 0.0;
    double denom = 0.0;
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            // Get the coefficients
            Coefficients c = node_P->get_momentum_coefficients(VelocityComponent::U);

            // Solve for velocity_u_P
            double velocity_u_P = c.source;
            for (int dir = direction_start; dir < direction_all_end; dir++) {
                const Direction direction = static_cast<Direction>(dir);
                Face *face = node_P->get_neighbouring_face(direction);
                if (face != nullptr && face->get_face_type() != FaceType::Boundary) {
                    const Node *neighbouring_node = node_P->get_neighbouring_node(direction);
                    velocity_u_P += c.get_coefficient(direction) * neighbouring_node->get_velocity_u();
                }
            }

            const double residual = std::abs(c.center * node_P->get_velocity_u() - velocity_u_P);
            m_momentum_x_error += residual;
            denom += std::abs(c.center * node_P->get_velocity_u());
        }
    }
    if (denom != 0.0) {
        m_momentum_x_error /= denom;
    }
}

void Simulation::solve_x_momentum() const {
    double tol = m_momentum_x_error / 1e3;
    double momentum_x_error = 1.0;
    while (momentum_x_error > tol) {
        momentum_x_error = 0.0;
        double denom = 0.0;
        for (int i = 0; i < m_mesh->get_size_x(); i++) {
            for (int j = 0; j < m_mesh->get_size_y(); j++) {
                Node *node_P = m_mesh->get_node(i, j);

                // Nothing to calculate for an empty node
                if (node_P == nullptr) {
                    continue;
                }

                // Get the coefficients
                Coefficients c = node_P->get_momentum_coefficients(VelocityComponent::U);

                // Solve for velocity_u_P
                double velocity_u_P = c.source;
                for (int dir = direction_start; dir < direction_all_end; dir++) {
                    const Direction direction = static_cast<Direction>(dir);
                    Face *face = node_P->get_neighbouring_face(direction);
                    if (face != nullptr && face->get_face_type() != FaceType::Boundary) {
                        const Node *neighbouring_node = node_P->get_neighbouring_node(direction);
                        velocity_u_P += c.get_coefficient(direction) * neighbouring_node->get_velocity_u();
                    }
                }

                const double residual = std::abs(c.center * node_P->get_velocity_u() - velocity_u_P);
                momentum_x_error += residual;
                denom += std::abs(c.center * node_P->get_velocity_u());

                velocity_u_P /= c.center;
                node_P->set_velocity_u(velocity_u_P);
            }
        }
        if (denom != 0.0) {
            momentum_x_error /= denom;
        }
    }
}

void Simulation::calculate_momentum_y_imbalance() {
    m_momentum_y_error = 0.0;
    double denom = 0.0;
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            // Get the coefficients
            Coefficients c = node_P->get_momentum_coefficients(VelocityComponent::V);

            // Solve for velocity_v_P
            double velocity_v_P = c.source;
            for (int dir = direction_start; dir < direction_all_end; dir++) {
                const Direction direction = static_cast<Direction>(dir);
                Face *face = node_P->get_neighbouring_face(direction);
                if (face != nullptr && face->get_face_type() != FaceType::Boundary) {
                    const Node *neighbouring_node = node_P->get_neighbouring_node(direction);
                    velocity_v_P += c.get_coefficient(direction) * neighbouring_node->get_velocity_v();
                }
            }

            const double residual = std::abs(c.center * node_P->get_velocity_v() - velocity_v_P);
            m_momentum_y_error += residual;
            denom += std::abs(c.center * node_P->get_velocity_v());
        }
    }
    if (denom != 0.0) {
        m_momentum_y_error /= denom;
    }
}

void Simulation::solve_y_momentum() const {
    double tol = m_momentum_y_error / 1e3;
    double momentum_y_error = 1.0;
    while (momentum_y_error > tol) {
        momentum_y_error = 0.0;
        double denom = 0.0;
        for (int i = 0; i < m_mesh->get_size_x(); i++) {
            for (int j = 0; j < m_mesh->get_size_y(); j++) {
                Node *node_P = m_mesh->get_node(i, j);

                // Nothing to calculate for an empty node
                if (node_P == nullptr) {
                    continue;
                }

                // Get the coefficients
                Coefficients c = node_P->get_momentum_coefficients(VelocityComponent::V);

                // Solve for velocity_v_P
                double velocity_v_P = c.source;
                for (int dir = direction_start; dir < direction_all_end; dir++) {
                    const Direction direction = static_cast<Direction>(dir);
                    Face *face = node_P->get_neighbouring_face(direction);
                    if (face != nullptr && face->get_face_type() != FaceType::Boundary) {
                        const Node *neighbouring_node = node_P->get_neighbouring_node(direction);
                        velocity_v_P += c.get_coefficient(direction) * neighbouring_node->get_velocity_v();
                    }
                }

                const double residual = std::abs(c.center * node_P->get_velocity_v() - velocity_v_P);
                momentum_y_error += residual;
                denom += std::abs(c.center * node_P->get_velocity_v());

                velocity_v_P /= c.center;
                node_P->set_velocity_v(velocity_v_P);
            }
        }
        if (denom != 0.0) {
            momentum_y_error /= denom;
        }
    }
}

void Simulation::calculate_mass_imbalance() {
    m_mass_imbalance = 0.0;
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            const Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            const double residual = std::abs(node_P->get_pressure_coefficient(CoefficientType::Source));
            m_mass_imbalance = std::max(m_mass_imbalance, residual);
        }
    }

    // Update the residual normalization factor
    if (can_update_mass_imbalance_residual_normalization_factor) {
        m_mass_imbalance_residual_normalization_factor = std::max(m_mass_imbalance_residual_normalization_factor,
                                                                  m_mass_imbalance);
        if (m_outer_iterations_count >= residual_normalization_iterations) {
            can_update_mass_imbalance_residual_normalization_factor = false;
        }
    }

    // Apply the normalization to the residual
    if (m_mass_imbalance_residual_normalization_factor != 0.0) {
        m_mass_imbalance /= m_mass_imbalance_residual_normalization_factor;
    }
}

void Simulation::solve_pressure_correction() const {
    // Initialize the pressure correction
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->set_pressure_correction(0.0);
        }
    }

    // Solve pressure correction equation
    const double tol = 1e-4; // 4 orders of magnitude
    double pressure_correction_error = 1.0;
    while (pressure_correction_error > tol) {
        pressure_correction_error = 0.0;
        double denom = 0.0;
        for (int i = 0; i < m_mesh->get_size_x(); i++) {
            for (int j = 0; j < m_mesh->get_size_y(); j++) {
                Node *node_P = m_mesh->get_node(i, j);

                // Nothing to calculate for an empty node
                if (node_P == nullptr) {
                    continue;
                }

                // Get the coefficients
                Coefficients c = node_P->get_pressure_coefficients();

                // Solve for pressure_correction_P
                double pressure_correction_P = c.source;
                for (int dir = direction_start; dir < direction_near_end; dir++) {
                    const Direction direction = static_cast<Direction>(dir);
                    const Face *face = node_P->get_neighbouring_face(direction);
                    if (face->get_face_type() != FaceType::Boundary) {
                        const Node *neighbouring_node = node_P->get_neighbouring_node(direction);
                        pressure_correction_P += c.get_coefficient(direction) * neighbouring_node->
                                get_pressure_correction();
                    }
                }

                const double residual = std::pow(pressure_correction_P - c.center * node_P->get_pressure_correction(),
                                                 2);
                pressure_correction_error += residual;
                denom += std::pow(pressure_correction_P, 2);

                pressure_correction_P /= c.center;
                node_P->set_pressure_correction(pressure_correction_P);
            }
        }
        if (denom != 0.0) {
            pressure_correction_error /= denom;
        }
    }
}

void Simulation::simple_iterate(const SimulationType simulation_type) {
    // Calculate the momentum coefficients
    m_bulk_node_operations->calculate_momentum_coefficients(VelocityComponent::U, simulation_type);
    m_bulk_node_operations->calculate_momentum_coefficients(VelocityComponent::V, simulation_type);

    // Calculate the momentum imbalance
    calculate_momentum_x_imbalance();
    calculate_momentum_y_imbalance();

    // Solve X and Y momentum equations
    solve_x_momentum();
    solve_y_momentum();

    // Calculate the face velocities
    m_bulk_face_operations->update_face_x_velocities_rhie_chow();
    m_bulk_face_operations->update_face_y_velocities_rhie_chow();

    // Calculate the pressure correction coefficients
    m_bulk_node_operations->calculate_pressure_coefficients();

    // Calculate the mass imbalance
    calculate_mass_imbalance();

    // Solve pressure correction equation (on the nodes)
    solve_pressure_correction();

    // Update the pressure correction on the faces
    m_bulk_face_operations->update_face_x_pressure_corrections();
    m_bulk_face_operations->update_face_y_pressure_corrections();

    // Correct the u and v node velocities
    m_bulk_node_operations->correct_node_velocity_u();
    m_bulk_node_operations->correct_node_velocity_v();

    // Correct the face x and y velocities
    m_bulk_face_operations->correct_face_x_velocity();
    m_bulk_face_operations->correct_face_y_velocity();

    // Correct the pressure on the nodes
    m_bulk_node_operations->correct_node_pressure();

    // Update the pressure on the faces
    m_bulk_face_operations->update_face_x_pressures();
    m_bulk_face_operations->update_face_y_pressures();

    m_outer_iterations_count++;
}
