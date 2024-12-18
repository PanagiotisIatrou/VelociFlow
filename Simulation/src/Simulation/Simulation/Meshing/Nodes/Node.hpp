#pragma once

#include <array>
#include <memory>

#include "../../../common.hpp"
#include "../Faces/Face.hpp"
#include "Coefficients/MomentumCoefficients/MomentumCoefficients.hpp"
#include "Coefficients/PressureCorrectionCoefficients/PressureCorrectionCoefficients.hpp"

class Node {
protected:
    double m_velocity_u;
    double m_velocity_v;
    double m_pressure;
    double m_previous_timestep_velocity_u;
    double m_previous_timestep_velocity_v;
    double m_previous_timestep_pressure;
    double m_viscosity = 0.0;
    double m_density = 0.0;
    double m_dx = 0.0; // TODO: Remove in the future
    double m_dy = 0.0; // TODO: Remove in the future
    double m_dt = 1.0;
    double m_pressure_correction = 0.0;

    std::unique_ptr<MomentumCoefficients> m_momentum_coefficients;
    std::unique_ptr<PressureCorrectionCoefficients> m_pressure_correction_coefficients;

    std::array<Face *, 8> m_neighbouring_faces = {
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
    };
    std::array<Node *, 8> m_neighbouring_nodes = {
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
    };

public:
    Node(double viscosity, double density, double dx, double dy, double velocity_u, double velocity_v, double pressure);

    void set_velocity_u(double velocity_u);

    double get_velocity_u() const;

    void set_previous_timestep_velocity_u(double velocity);

    double get_previous_timestep_velocity_u() const;

    void set_velocity_v(double velocity_v);

    double get_velocity_v() const;

    void set_previous_timestep_velocity_v(double velocity);

    double get_previous_timestep_velocity_v() const;

    void set_pressure(double pressure);

    double get_pressure() const;

    void set_previous_timestep_pressure(double pressure);

    double get_previous_timestep_pressure() const;

    double get_dx() const;

    double get_dy() const;

    double get_dt() const;

    void set_dt(double dt);

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

    void calculate_momentum_coefficients(VelocityComponent velocity_component, SimulationType simulation_type) const;

    double get_momentum_coefficient(CoefficientType type, VelocityComponent velocity_component) const;

    Coefficients get_momentum_coefficients(VelocityComponent velocity_component) const;

    void calculate_pressure_coefficients() const;

    Coefficients get_pressure_coefficients() const;

    double get_pressure_coefficient(CoefficientType type) const;
};
