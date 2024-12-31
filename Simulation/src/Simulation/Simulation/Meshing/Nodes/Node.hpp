#pragma once

#include <array>
#include <memory>

#include "../../../common.hpp"
#include "../Faces/Face.hpp"
#include "EquationCoefficients/ConvectionDiffusionCoefficients.hpp"
#include "EquationCoefficients/MomentumCoefficients.hpp"
#include "EquationCoefficients/PressureCorrectionCoefficients.hpp"
#include "EquationCoefficients/DyeCoefficients.hpp"

class Node {
protected:
    double m_velocity_x;
    double m_velocity_y;
    double m_pressure;
    double m_dye;
    double m_previous_timestep_velocity_x;
    double m_previous_timestep_velocity_y;
    double m_previous_timestep_pressure;
    double m_previous_timestep_dye;
    double m_viscosity;
    double m_density;
    double m_dx; // TODO: Remove in the future
    double m_dy; // TODO: Remove in the future
    double m_dt = 1.0;
    double m_pressure_correction = 0.0;

    std::unique_ptr<MomentumCoefficients> m_momentum_coefficients;
    std::unique_ptr<PressureCorrectionCoefficients> m_pressure_correction_coefficients;
    std::unique_ptr<DyeCoefficients> m_dye_coefficients;
    std::unique_ptr<ConvectionDiffusionCoefficients> m_convection_diffusion_coefficients;

    std::array<Face *, 8> m_neighbouring_faces = {
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
    };
    std::array<Node *, 8> m_neighbouring_nodes = {
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
    };

public:
    Node(double viscosity, double density, double dx, double dy, double velocity_x, double velocity_y, double pressure, double dye);

    double get_dx() const;

    double get_dy() const;

    double get_dt() const;

    void set_dt(double dt);

    double get_previous_timestep_variable_value(Variable variable) const;

    double set_previous_timestep_variable_value(Variable variable, double value);

    double get_field_value(Field field) const;

    void set_field_value(Field field, double value);

    void apply_pressure_correction();

    void correct_velocity_x();

    void correct_velocity_y();

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

    void calculate_dye_coefficients(SimulationType type) const;

    Coefficients get_dye_coefficients() const;

    double get_dye_coefficient(CoefficientType type) const;

    void calculate_convection_diffusion_coefficients(SimulationType type, VelocityComponent velocity_component) const;

    Coefficients get_convection_diffusion_coefficients(VelocityComponent velocity_component) const;

    double get_convection_diffusion_coefficient(CoefficientType type, VelocityComponent velocity_component) const;
};
