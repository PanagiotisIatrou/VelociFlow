#include "SteadySimulation.hpp"

#include <cmath>
#include <iostream>

#include "Meshing/Faces/Boundary/BoundaryFace.hpp"
#include "Meshing/Faces/Interior/InteriorFace.hpp"
#include "Utilities/Saver.h"

SteadySimulation::SteadySimulation(Mesh *mesh, const double velocity_u_tolerance, const double velocity_v_tolerance,
                                   const double pressure_tolerance) {
    m_mesh = mesh;
    m_bulk_node_operations = std::make_unique<BulkNodeOperations>(mesh);
    m_bulk_face_operations = std::make_unique<BulkFaceOperations>(mesh);
    m_timer = std::make_unique<Timer>();

    m_velocity_u_tolerance = velocity_u_tolerance;
    m_velocity_v_tolerance = velocity_v_tolerance;
    m_pressure_tolerance = pressure_tolerance;
}

double SteadySimulation::get_time_taken() const {
    return m_time_taken;
}

void SteadySimulation::calculate_active_cells_count() {
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

void SteadySimulation::solve() {
    m_timer->start_timer();

    calculate_active_cells_count();
    m_bulk_face_operations->update_face_x_velocities_distance_weighted();
    m_bulk_face_operations->update_face_y_velocities_distance_weighted();
    m_bulk_face_operations->update_face_x_viscosities();
    m_bulk_face_operations->update_face_y_viscosities();
    m_bulk_face_operations->update_face_x_pressures();
    m_bulk_face_operations->update_face_y_pressures();

    m_outer_iterations_count = 0;
    m_momentum_x_error = 1.0;
    m_momentum_y_error = 1.0;
    m_mass_imbalance = 1.0;
    while (m_momentum_x_error > m_velocity_u_tolerance || m_momentum_y_error > m_velocity_v_tolerance ||
           m_mass_imbalance > m_pressure_tolerance) {
        iterate();
        m_outer_iterations_count++;
        std::cout << m_momentum_x_error << ", " << m_momentum_y_error << ", " << m_mass_imbalance << std::endl;
    }
    m_time_taken = m_timer->get_elapsed_time();
    std::cout << "Converged in " << m_outer_iterations_count << " iterations" << std::endl;
}

void SteadySimulation::iterate() {
    // Calculate the momentum coefficients
    m_bulk_node_operations->calculate_momentum_coefficients(VelocityComponent::U);
    m_bulk_node_operations->calculate_momentum_coefficients(VelocityComponent::V);

    // Solve X and Y momentum equations
    solve_x_momentum();
    solve_y_momentum();

    // Calculate the face velocities
    m_bulk_face_operations->update_face_x_velocities_rhie_chow();
    m_bulk_face_operations->update_face_y_velocities_rhie_chow();
    // m_bulk_face_operations->update_face_x_velocities_distance_weighted();
    // m_bulk_face_operations->update_face_y_velocities_distance_weighted();
    // m_mass_imbalance = 0.0;

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

    // double imb[100][100];
    // double w1 = 0.0;
    // for (int i = 0; i < m_mesh->get_size_x(); i++) {
    //     for (int j = 0; j < m_mesh->get_size_y(); j++) {
    //         Node *node_P;
    //         if (i == m_mesh->get_size_x()) {
    //             node_P = m_mesh->get_node(m_mesh->get_size_x() - 1, j);
    //         } else {
    //             node_P = m_mesh->get_node(i, j);
    //         }
    //
    //         Face *face_w = node_P->get_neighbouring_face(Direction::West);
    //         Face *face_e = node_P->get_neighbouring_face(Direction::East);
    //         Face *face_s = node_P->get_neighbouring_face(Direction::South);
    //         Face *face_n = node_P->get_neighbouring_face(Direction::North);
    //
    //         // Velocity w
    //         double velocity_w;
    //         if (face_w->get_type() == FaceType::Boundary) {
    //             velocity_w = static_cast<BoundaryFace *>(face_w)->get_velocity_u();
    //         } else {
    //             velocity_w = static_cast<InteriorFace *>(face_w)->get_velocity();
    //         }
    //
    //         // Velocity e
    //         double velocity_e;
    //         if (face_e->get_type() == FaceType::Boundary) {
    //             velocity_e = static_cast<BoundaryFace *>(face_e)->get_velocity_u();
    //         } else {
    //             velocity_e = static_cast<InteriorFace *>(face_e)->get_velocity();
    //         }
    //
    //         // Velocity s
    //         double velocity_s;
    //         if (face_s->get_type() == FaceType::Boundary) {
    //             velocity_s = static_cast<BoundaryFace *>(face_s)->get_velocity_v();
    //         } else {
    //             velocity_s = static_cast<InteriorFace *>(face_s)->get_velocity();
    //         }
    //
    //         // Velocity n
    //         double velocity_n;
    //         if (face_n->get_type() == FaceType::Boundary) {
    //             velocity_n = static_cast<BoundaryFace *>(face_n)->get_velocity_v();
    //         } else {
    //             velocity_n = static_cast<InteriorFace *>(face_n)->get_velocity();
    //         }
    //
    //         imb[i][j] = std::abs(
    //             velocity_e * m_mesh->get_dy()
    //             - velocity_w * m_mesh->get_dy()
    //             + velocity_n * m_mesh->get_dx()
    //             - velocity_s * m_mesh->get_dx());
    //
    //         w1 += imb[i][j];
    //     }
    // }
    // w1 = std::sqrt(w1);

    // Correct the face x and y velocities
    m_bulk_face_operations->correct_face_x_velocity();
    m_bulk_face_operations->correct_face_y_velocity();

    // bool found = false;
    // double w2 = 0.0;
    // for (int i = 0; i < m_mesh->get_size_x(); i++) {
    //     for (int j = 0; j < m_mesh->get_size_y(); j++) {
    //         Node *node_P;
    //         if (i == m_mesh->get_size_x()) {
    //             node_P = m_mesh->get_node(m_mesh->get_size_x() - 1, j);
    //         } else {
    //             node_P = m_mesh->get_node(i, j);
    //         }
    //
    //         Face *face_w = node_P->get_neighbouring_face(Direction::West);
    //         Face *face_e = node_P->get_neighbouring_face(Direction::East);
    //         Face *face_s = node_P->get_neighbouring_face(Direction::South);
    //         Face *face_n = node_P->get_neighbouring_face(Direction::North);
    //
    //         // Velocity w
    //         double velocity_w;
    //         if (face_w->get_type() == FaceType::Boundary) {
    //             velocity_w = static_cast<BoundaryFace *>(face_w)->get_velocity_u();
    //         } else {
    //             velocity_w = static_cast<InteriorFace *>(face_w)->get_velocity();
    //         }
    //
    //         // Velocity e
    //         double velocity_e;
    //         if (face_e->get_type() == FaceType::Boundary) {
    //             velocity_e = static_cast<BoundaryFace *>(face_e)->get_velocity_u();
    //         } else {
    //             velocity_e = static_cast<InteriorFace *>(face_e)->get_velocity();
    //         }
    //
    //         // Velocity s
    //         double velocity_s;
    //         if (face_s->get_type() == FaceType::Boundary) {
    //             velocity_s = static_cast<BoundaryFace *>(face_s)->get_velocity_v();
    //         } else {
    //             velocity_s = static_cast<InteriorFace *>(face_s)->get_velocity();
    //         }
    //
    //         // Velocity n
    //         double velocity_n;
    //         if (face_n->get_type() == FaceType::Boundary) {
    //             velocity_n = static_cast<BoundaryFace *>(face_n)->get_velocity_v();
    //         } else {
    //             velocity_n = static_cast<InteriorFace *>(face_n)->get_velocity();
    //         }
    //
    //         double imbalance = std::abs(
    //             velocity_e * m_mesh->get_dy()
    //             - velocity_w * m_mesh->get_dy()
    //             + velocity_n * m_mesh->get_dx()
    //             - velocity_s * m_mesh->get_dx());
    //
    //         w2 += imbalance;
    //
    //         if (imbalance > imb[i][j] && !found) {
    //             found = true;
    //             std::cerr << "1. Imbalance increased from " << imb[i][j] << " to " << imbalance << std::endl;
    //         }
    //     }
    // }
    // w2 = std::sqrt(w2);
    //
    // if (w2 > w1) {
    //     std::cerr << "2. Imbalance increased from " << w1 << " to " << w2 << std::endl;
    // }

    // Correct the pressure on the nodes
    m_bulk_node_operations->correct_node_pressure();

    // Update the pressure on the faces
    m_bulk_face_operations->update_face_x_pressures();
    m_bulk_face_operations->update_face_y_pressures();
}

void SteadySimulation::solve_x_momentum() {
    // Calculate the residual scaling factor
    double velocity_u_mean = 0.0;
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            if (node_P == nullptr) {
                continue;
            }

            velocity_u_mean += node_P->get_velocity_u();
        }
    }
    velocity_u_mean /= m_active_cells_count;
    double representative_difference = 0.0;
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            if (node_P == nullptr) {
                continue;
            }

            representative_difference += std::abs(node_P->get_velocity_u() - velocity_u_mean);
        }
    }
    representative_difference /= m_active_cells_count;

    if (m_outer_iterations_count == 0) {
        representative_difference = 1.0;
    }

    double tol = 0.0;
    double momentum_x_error = 1.0;
    int iterations_count = 0;
    while (momentum_x_error > tol) {
        momentum_x_error = 0.0;
        for (int i = 0; i < m_mesh->get_size_x(); i++) {
            for (int j = 0; j < m_mesh->get_size_y(); j++) {
                Node *node_P = m_mesh->get_node(i, j);

                // Nothing to calculate for an empty node
                if (node_P == nullptr) {
                    continue;
                }

                // Get the coefficients
                std::array<double, 6> c = node_P->get_momentum_coefficients(VelocityComponent::U);
                const double a_P = c[0];
                const double source = c[1];
                const double a_W = c[2];
                const double a_E = c[3];
                const double a_S = c[4];
                const double a_N = c[5];

                // Solve for velocity_u_P
                double velocity_u_P = source;

                Face *face_w = node_P->get_neighbouring_face(Direction::West);
                if (face_w->get_type() != FaceType::Boundary) {
                    Node *node_W = m_mesh->get_node(i - 1, j);
                    velocity_u_P += a_W * node_W->get_velocity_u();
                }

                Face *face_e = node_P->get_neighbouring_face(Direction::East);
                if (face_e->get_type() != FaceType::Boundary) {
                    Node *node_E = m_mesh->get_node(i + 1, j);
                    velocity_u_P += a_E * node_E->get_velocity_u();
                }

                Face *face_s = node_P->get_neighbouring_face(Direction::South);
                if (face_s->get_type() != FaceType::Boundary) {
                    Node *node_S = m_mesh->get_node(i, j - 1);
                    velocity_u_P += a_S * node_S->get_velocity_u();
                }

                Face *face_n = node_P->get_neighbouring_face(Direction::North);
                if (face_n->get_type() != FaceType::Boundary) {
                    Node *node_N = m_mesh->get_node(i, j + 1);
                    velocity_u_P += a_N * node_N->get_velocity_u();
                }

                // if (iterations_count == 0)
                double residual = (velocity_u_P - a_P * node_P->get_velocity_u()) / (a_P * representative_difference);
                momentum_x_error += std::pow(residual, 2);
                velocity_u_P /= a_P;

                // if (iterations_count > 0)
                    // velocity_u_error += std::pow(velocity_u_P - node_P->get_velocity_u(), 2);
                node_P->set_velocity_u(velocity_u_P);
            }
        }
        momentum_x_error = std::sqrt(momentum_x_error / m_active_cells_count);
        if (iterations_count == 0) {
            m_momentum_x_error = momentum_x_error;
            tol = m_momentum_x_error / 1e1; // 1 orders of magnitude
        }
        iterations_count++;
    }
}

void SteadySimulation::solve_y_momentum() {
    // Calculate the residual scaling factor
    double velocity_v_mean = 0.0;
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            if (node_P == nullptr) {
                continue;
            }

            velocity_v_mean += node_P->get_velocity_v();
        }
    }
    velocity_v_mean /= m_active_cells_count;
    double representative_difference = 0.0;
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            if (node_P == nullptr) {
                continue;
            }

            representative_difference += std::abs(node_P->get_velocity_v() - velocity_v_mean);
        }
    }
    representative_difference /= m_active_cells_count;

    if (m_outer_iterations_count == 0) {
        representative_difference = 1.0;
    }

    double tol = 0.0;
    double momentum_y_error = 1.0;
    int iterations_count = 0;
    while (momentum_y_error > tol) {
        momentum_y_error = 0.0;
        for (int i = 0; i < m_mesh->get_size_x(); i++) {
            for (int j = 0; j < m_mesh->get_size_y(); j++) {
                Node *node_P = m_mesh->get_node(i, j);

                // Nothing to calculate for an empty node
                if (node_P == nullptr) {
                    continue;
                }

                // Get the coefficients
                std::array<double, 6> c = node_P->get_momentum_coefficients(VelocityComponent::V);
                const double a_P = c[0];
                const double source = c[1];
                const double a_W = c[2];
                const double a_E = c[3];
                const double a_S = c[4];
                const double a_N = c[5];

                // Solve for velocity_v_P
                double velocity_v_P = source;

                Face *face_w = node_P->get_neighbouring_face(Direction::West);
                if (face_w->get_type() != FaceType::Boundary) {
                    Node *node_W = m_mesh->get_node(i - 1, j);
                    velocity_v_P += a_W * node_W->get_velocity_v();
                }

                Face *face_e = node_P->get_neighbouring_face(Direction::East);
                if (face_e->get_type() != FaceType::Boundary) {
                    Node *node_E = m_mesh->get_node(i + 1, j);
                    velocity_v_P += a_E * node_E->get_velocity_v();
                }

                Face *face_s = node_P->get_neighbouring_face(Direction::South);
                if (face_s->get_type() != FaceType::Boundary) {
                    Node *node_S = m_mesh->get_node(i, j - 1);
                    velocity_v_P += a_S * node_S->get_velocity_v();
                }

                Face *face_n = node_P->get_neighbouring_face(Direction::North);
                if (face_n->get_type() != FaceType::Boundary) {
                    Node *node_N = m_mesh->get_node(i, j + 1);
                    velocity_v_P += a_N * node_N->get_velocity_v();
                }

                // if (iterations_count == 0)
                double residual = (velocity_v_P - a_P * node_P->get_velocity_v()) / (a_P * representative_difference);
                momentum_y_error += std::pow(residual, 2);
                velocity_v_P /= a_P;

                // if (iterations_count > 0)
                    // velocity_v_error += std::pow(velocity_v_P - node_P->get_velocity_v(), 2);
                node_P->set_velocity_v(velocity_v_P);
            }
        }
        momentum_y_error = std::sqrt(momentum_y_error / m_active_cells_count);
        if (iterations_count == 0) {
            m_momentum_y_error = momentum_y_error;
            tol = m_momentum_y_error / 1e1; // 1 orders of magnitude
        }
        iterations_count++;
    }
}

void SteadySimulation::calculate_mass_imbalance() {
    // Calculate the mass imbalance
    m_mass_imbalance = 0.0;
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            const Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            const double source = node_P->get_pressure_coefficient(CoefficientType::Source);
            m_mass_imbalance += std::pow(source, 2);
        }
    }
    m_mass_imbalance = std::sqrt(m_mass_imbalance / m_active_cells_count);
}

void SteadySimulation::solve_pressure_correction() const {
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
    const double tol = 1e-2; // 2 orders of magnitude
    double pressure_correction_error = 1.0;
    while (pressure_correction_error > tol) {
        pressure_correction_error = 0.0;
        for (int i = 0; i < m_mesh->get_size_x(); i++) {
            for (int j = 0; j < m_mesh->get_size_y(); j++) {
                Node *node_P = m_mesh->get_node(i, j);

                // Nothing to calculate for an empty node
                if (node_P == nullptr) {
                    continue;
                }

                // Get the coefficients
                std::array<double, 6> c = node_P->get_pressure_coefficients();
                const double a_P = c[0];
                const double source = c[1];
                const double a_W = c[2];
                const double a_E = c[3];
                const double a_S = c[4];
                const double a_N = c[5];

                // Solve for pressure_correction_P
                double pressure_correction_P = source;

                const Face *face_w = node_P->get_neighbouring_face(Direction::West);
                if (face_w->get_type() != FaceType::Boundary) {
                    pressure_correction_P += a_W * node_P->get_neighbouring_node(Direction::West)->
                            get_pressure_correction();
                }

                const Face *face_e = node_P->get_neighbouring_face(Direction::East);
                if (face_e->get_type() != FaceType::Boundary) {
                    pressure_correction_P += a_E * node_P->get_neighbouring_node(Direction::East)->
                            get_pressure_correction();
                }

                const Face *face_s = node_P->get_neighbouring_face(Direction::South);
                if (face_s->get_type() != FaceType::Boundary) {
                    pressure_correction_P += a_S * node_P->get_neighbouring_node(Direction::South)->
                            get_pressure_correction();
                }

                const Face *face_n = node_P->get_neighbouring_face(Direction::North);
                if (face_n->get_type() != FaceType::Boundary) {
                    pressure_correction_P += a_N * node_P->get_neighbouring_node(Direction::North)->
                            get_pressure_correction();
                }

                pressure_correction_P /= a_P;
                pressure_correction_error += std::pow(pressure_correction_P - node_P->get_pressure_correction(), 2);
                node_P->set_pressure_correction(pressure_correction_P);
            }
        }
        pressure_correction_error = std::sqrt(pressure_correction_error / m_active_cells_count);
    }
}

void SteadySimulation::save_results(const std::string &path) const {
    Saver::save_mesh_data(path, m_time_taken, m_mesh);
}
