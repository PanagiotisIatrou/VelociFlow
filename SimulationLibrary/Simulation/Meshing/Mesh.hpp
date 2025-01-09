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

    void set_node(int i, int j, double velocity_x, double velocity_y, double pressure, double dye);

    Node *get_node(int i, int j) const;

    Face *get_face_x(int i, int j) const;

    Face *get_face_y(int i, int j) const;

    // Face types

    void set_interior_face(FaceSide side, int i, int j);

    void set_boundary_inlet_face(FaceSide side, int i, int j, double velocity_x, double velocity_y, double dye);

    void set_boundary_no_slip_face(FaceSide side, int i, int j);

    void set_boundary_moving_wall_face(FaceSide side, int i, int j, double velocity);

    void set_boundary_slip_face(FaceSide side, int i, int j);

    void set_boundary_fixed_pressure_face(FaceSide side, int i, int j, double pressure);

    void set_boundary_free_face(FaceSide side, int i, int j);

    // Linking

    void link_nodes() const;

    void link_nodes_faces();
};
