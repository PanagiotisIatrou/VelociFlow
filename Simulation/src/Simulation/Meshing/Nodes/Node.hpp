#pragma once

#include <array>
#include "../Faces/Face.hpp"

enum class Direction { West = 0, East = 1, South = 2, North = 3 };

enum class VelocityComponent { U, V };

enum class CoefficientType { Center = 0, Source = 1, West = 2, East = 3, South = 4, North = 5 };

class PressureCoefficients;

class Node {
protected:
    double m_velocity_u = 0.0;
    double m_velocity_v = 0.0;
    double m_pressure = 0.0;
    double m_viscosity = 0.0;
    double m_density = 0.0;
    double m_dx = 0.0; // TODO: Remove in the future
    double m_dy = 0.0; // TODO: Remove in the future
    double m_pressure_correction = 0.0;

    std::array<Face *, 4> m_neighbouring_faces = {nullptr, nullptr, nullptr, nullptr};
    std::array<Node *, 4> m_neighbouring_nodes = {nullptr, nullptr, nullptr, nullptr};

    std::array<double, 6> m_momentum_u_coefficients = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    std::array<double, 6> m_momentum_v_coefficients = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    std::array<double, 6> m_pressure_coefficients = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    std::array<double, 6> get_diffusion_effects(VelocityComponent velocity_component) const;

    std::array<double, 6> get_convection_effects(VelocityComponent velocity_component) const;

    std::array<double, 6> get_pressure_effects(VelocityComponent velocity_component) const;

public:
    Node(double density, double viscosity, double dx, double dy);

    void set_velocity_u(double velocity_u);

    double get_velocity_u() const;

    void set_velocity_v(double velocity_v);

    double get_velocity_v() const;

    void set_pressure(double pressure);

    double get_pressure() const;

    double get_viscosity() const;

    double get_density() const;

    double get_pressure_correction() const;

    void set_pressure_correction(double pressure_correction);

    void apply_pressure_correction();

    void correct_velocity_u();

    void correct_velocity_v();

    Face *get_neighbouring_face(Direction direction) const;

    void set_neighbouring_face(Face *face, Direction direction);

    Node *get_neighbouring_node(Direction direction) const;

    void set_neighbouring_node(Node *node, Direction direction);

    void calculate_momentum_coefficients(VelocityComponent velocity_component);

    double get_momentum_coefficient(CoefficientType type, VelocityComponent velocity_component) const;

    std::array<double, 6> get_momentum_coefficients(VelocityComponent velocity_component) const;

    void calculate_pressure_coefficients();

    std::array<double, 6> get_pressure_coefficients() const;

    double get_pressure_coefficient(CoefficientType type) const;
};
