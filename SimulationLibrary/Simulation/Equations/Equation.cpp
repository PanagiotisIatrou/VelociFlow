#include "Equation.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "Coefficients.hpp"

Equation::Equation(Mesh *mesh, const EquationType equation_type, const Field variable_field,
                   const double relaxation_factor, const ResidualType residual_type, const StoppingRule stopping_rule,
                   const NormType norm_type, const double stopping_tolerance, const bool include_time) {
    m_mesh = mesh;
    m_equation_type = equation_type;
    m_variable_field = variable_field;
    m_relaxation_factor = relaxation_factor;
    m_residual_type = residual_type;
    m_stopping_rule = stopping_rule;
    m_norm_type = norm_type;
    m_stopping_tolerance = stopping_tolerance;
    m_include_time = include_time;

    reset_imbalance();
}

void Equation::populate_mesh() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->add_equation_coefficient(m_equation_type, m_variable_field, m_relaxation_factor, m_include_time);
        }
    }
}

void Equation::calculate_coefficients() const {
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            node_P->calculate_equation_coefficients(m_equation_type);
        }
    }
}

void Equation::solve() {
    double tol;
    if (m_stopping_rule == StoppingRule::Absolute) {
        tol = m_stopping_tolerance;
    } else if (m_stopping_rule == StoppingRule::Relative) {
        tol = std::max(m_imbalance / (1.0 / m_stopping_tolerance), 1e-10);
    } else {
        std::cerr << "Stopping rule not implemented" << std::endl;
        exit(1);
    }

    double error = std::numeric_limits<double>::infinity();
    while (error > tol) {
        for (int i = 0; i < m_mesh->get_size_x(); i++) {
            for (int j = 0; j < m_mesh->get_size_y(); j++) {
                Node *node_P = m_mesh->get_node(i, j);

                // Nothing to calculate for an empty node
                if (node_P == nullptr) {
                    continue;
                }

                // Get the coefficients
                Coefficients c = node_P->get_equation_coefficients(m_equation_type);

                // Solve for the nodal value (the main diagonal variable)
                double value_P = c.source;
                for (int dir = direction_start; dir < direction_all_end; dir++) {
                    const Direction direction = static_cast<Direction>(dir);
                    Face *face = node_P->get_neighbouring_face(direction);
                    if (face != nullptr && face->get_face_type() != FaceType::Boundary) {
                        const Node *neighbouring_node = node_P->get_neighbouring_node(direction);
                        value_P += c.get_coefficient(direction) * neighbouring_node->get_field_value(m_variable_field);
                    }
                }

                value_P /= c.center;

                // Apply the value limits
                value_P = std::clamp(value_P, m_lower_value_limit, m_upper_value_limit);

                node_P->set_field_value(m_variable_field, value_P);
            }
        }

        error = calculate_and_get_imbalance();
    }

    m_iterations_count++;
}

double Equation::calculate_and_get_imbalance() {
    double error = 0.0;
    double scaled_denominator = 0.0;
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            const Node *node_P = m_mesh->get_node(i, j);

            // Nothing to calculate for an empty node
            if (node_P == nullptr) {
                continue;
            }

            // Get the coefficients
            Coefficients c = node_P->get_equation_coefficients(m_equation_type);

            // Solve for the nodal value (the main diagonal variable)
            double value_P = c.source;
            for (int dir = direction_start; dir < direction_all_end; dir++) {
                const Direction direction = static_cast<Direction>(dir);
                Face *face = node_P->get_neighbouring_face(direction);
                if (face != nullptr && face->get_face_type() != FaceType::Boundary) {
                    const Node *neighbouring_node = node_P->get_neighbouring_node(direction);
                    value_P += c.get_coefficient(direction) * neighbouring_node->get_field_value(m_variable_field);
                }
            }

            if (m_norm_type == NormType::L1) {
                const double residual = std::abs(c.center * node_P->get_field_value(m_variable_field) - value_P);
                error += residual;
                scaled_denominator += std::abs(c.center * node_P->get_field_value(m_variable_field));
            } else if (m_norm_type == NormType::L2) {
                const double residual = std::pow(c.center * node_P->get_field_value(m_variable_field) - value_P, 2);
                error += residual;
                scaled_denominator += std::pow(c.center * node_P->get_field_value(m_variable_field), 2);
            } else if (m_norm_type == NormType::LInfinity) {
                const double residual = std::abs(c.center * node_P->get_field_value(m_variable_field) - value_P);
                error = std::max(error, residual);
                scaled_denominator =
                    std::max(scaled_denominator, std::abs(c.center * node_P->get_field_value(m_variable_field)));
            } else {
                std::cerr << "Norm type not implemented" << std::endl;
                exit(1);
            }
        }
    }

    if (m_residual_type == ResidualType::Scaled && scaled_denominator != 0.0) {
        error /= std::max(scaled_denominator, 1e-10);
    } else if (m_residual_type == ResidualType::Normalized) {
        if (m_can_update_imbalance_normalization_factor) {
            m_imbalance_normalization_factor = std::max(m_imbalance_normalization_factor, error);

            if (m_iterations_count >= imbalance_normalization_iterations) {
                m_can_update_imbalance_normalization_factor = false;
            }
        }

        if (m_imbalance_normalization_factor != 0.0) {
            error /= std::max(m_imbalance_normalization_factor, 1e-10);
        }
    }

    return error;
}

void Equation::calculate_imbalance() {
    m_imbalance = calculate_and_get_imbalance();
}

double Equation::get_imbalance() const {
    return m_imbalance;
}

void Equation::reset_imbalance() {
    m_imbalance = std::numeric_limits<double>::infinity();
}

double Equation::get_imbalance_normalization_factor() const {
    return m_imbalance_normalization_factor;
}

void Equation::set_imbalance_normalization_factor(const double factor) {
    m_imbalance_normalization_factor = factor;
    m_can_update_imbalance_normalization_factor = false;
}

double Equation::get_tolerance() const {
    return m_stopping_tolerance;
}

void Equation::set_lower_value_limit(const double limit) {
    m_lower_value_limit = limit;
}

void Equation::set_upper_value_limit(const double limit) {
    m_upper_value_limit = limit;
}

void Equation::set_value_limits(const double lower_limit, const double upper_limit) {
    m_lower_value_limit = lower_limit;
    m_upper_value_limit = upper_limit;
}
