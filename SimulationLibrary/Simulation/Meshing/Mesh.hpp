#pragma once

#include <memory>
#include <vector>

#include "Nodes/Node.hpp"

enum class FaceSide { X, Y };

class Mesh {
private:
    int m_size_x;
    int m_size_y;

    double m_domain_size_x;
    double m_domain_size_y;

    double m_dx;
    double m_dy;
    double m_dt;

    std::vector<std::vector<std::unique_ptr<Node>>> m_nodes;
    std::vector<std::vector<std::unique_ptr<Face>>> m_faces_x;
    std::vector<std::vector<std::unique_ptr<Face>>> m_faces_y;

    double m_density = 0.0;
    double m_viscosity = 0.0;

public:
    Mesh(int size_x, int size_y, double domain_size_x, double domain_size_y);

    int get_size_x() const;

    int get_size_y() const;

    double get_dx() const;

    double get_dy() const;

    void set_dt(double dt);

    double get_dt() const;

    double get_domain_size_x() const;

    double get_domain_size_y() const;

    void set_node(int x, int y, FieldValues field_values);

    Node *get_node(int x, int y) const;

    Face *get_face_x(int x, int y) const;

    Face *get_face_y(int x, int y) const;

    // Face types

    void set_interior_face(FaceSide side, int x, int y);

    void set_boundary_fixed_value_face(FaceSide side, int x, int y, double phi);

    void set_boundary_inlet_face(FaceSide side, int x, int y, double velocity_x, double velocity_y, double dye);

    void set_boundary_no_slip_face(FaceSide side, int x, int y);

    void set_boundary_moving_wall_face(FaceSide side, int x, int y, double velocity);

    void set_boundary_slip_face(FaceSide side, int x, int y);

    void set_boundary_fixed_pressure_face(FaceSide side, int x, int y, double pressure);

    void set_boundary_periodic_side(FaceSide side);

    void set_boundary_free_face(FaceSide side, int x, int y);

    // Linking

    void link_nodes() const;

    void link_nodes_faces();

    // Density

    double get_density();

    // Viscosity

    double get_viscosity();
};
