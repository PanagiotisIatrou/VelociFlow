#pragma once

#include <array>
#include <map>
#include <memory>

#include "../../../common.hpp"
#include "../../Equations/Equations/EquationCoefficients/EquationCoefficients.hpp"
#include "../Faces/Face.hpp"

struct FieldValues {
    double velocity_x = 0.0;
    double velocity_y = 0.0;
    double pressure = 0.0;
    double dye = 0.0;
    double pressure_correction = 0.0;
    double phi = 0.0;
};

class Node {
protected:
    double m_velocity_x;
    double m_velocity_y;
    double m_pressure;
    double m_dye;
    double m_pressure_correction;
    double m_phi;
    double m_previous_timestep_velocity_x;
    double m_previous_timestep_velocity_y;
    double m_previous_timestep_pressure;
    double m_previous_timestep_dye;
    double m_previous_timestep_pressure_correction;
    double m_previous_timestep_phi;

    double m_density;
    double m_viscosity;

    double m_dx;
    double m_dy;
    double m_dt = 1.0;

    std::map<EquationType, std::unique_ptr<EquationCoefficients>> m_equation_coefficients;

    std::array<Face *, 8> m_neighbouring_faces = {nullptr, nullptr, nullptr, nullptr,
                                                  nullptr, nullptr, nullptr, nullptr};
    std::array<Node *, 8> m_neighbouring_nodes = {nullptr, nullptr, nullptr, nullptr,
                                                  nullptr, nullptr, nullptr, nullptr};

public:
    Node(double dx, double dy, FieldValues field_values);

    double get_dx() const;

    double get_dy() const;

    double get_dt() const;

    void set_dt(double dt);

    double get_viscosity() const;

    void set_viscosity(double viscosity);

    double get_density() const;

    void set_density(double density);

    double get_previous_timestep_field_value(Field field) const;

    void set_previous_timestep_field_value(Field field, double value);

    double get_field_value(Field field) const;

    void set_field_value(Field field, double value);

    void apply_pressure_correction();

    void correct_velocity_x();

    void correct_velocity_y();

    Face *get_neighbouring_face(Direction direction) const;

    void set_neighbouring_face(Face *face, Direction direction);

    Node *get_neighbouring_node(Direction direction) const;

    void set_neighbouring_node(Node *node, Direction direction);

    void add_equation_coefficient(EquationType equation_type, Field variable_field, double relaxation,
                                  bool include_time);

    double get_equation_coefficient(EquationType equation_type, CoefficientType coefficient_type) const;

    Coefficients get_equation_coefficients(EquationType equation_type) const;

    void calculate_equation_coefficients(EquationType equation_type);
};
