#include "Mesh.hpp"

#include <memory>

#include "Faces/Boundary/BoundaryFace.hpp"
#include "Faces/Boundary/FixedPressureBoundaryFace.hpp"
#include "Faces/Boundary/InletBoundaryFace.hpp"
#include "Faces/Boundary/FreeBoundaryFace.hpp"
#include "Faces/Boundary/MovingWallBoundaryFace.hpp"
#include "Faces/Boundary/NoSlipBoundaryFace.hpp"
#include "Faces/Boundary/SlipBoundaryFace.hpp"
#include "Faces/Interior/InteriorFaceX.hpp"
#include "Faces/Interior/InteriorFaceY.hpp"

Mesh::Mesh(const int size_x, const int size_y, const double domain_size_x, const double domain_size_y) {
    m_size_x = size_x;
    m_size_y = size_y;
    m_domain_size_x = domain_size_x;
    m_domain_size_y = domain_size_y;

    m_dx = domain_size_x / size_x;
    m_dy = domain_size_y / size_y;

    // Initialize all nodes to nullptr
    m_nodes.reserve(size_x);
    for (int i = 0; i < size_x; i++) {
        m_nodes[i].reserve(size_y);
    }

    // Initialize all the x faces to nullptr
    m_faces_x.reserve(size_x + 1);
    for (int i = 0; i < size_x + 1; i++) {
        m_faces_x[i].reserve(size_y);
        for (int j = 0; j < size_y; j++) {
            m_faces_x[i][j] = nullptr;
        }
    }

    // Initialize all the y faces to nullptr
    m_faces_y.reserve(size_x);
    for (int i = 0; i < size_x; i++) {
        m_faces_y[i].reserve(size_y + 1);
        for (int j = 0; j < size_y + 1; j++) {
            m_faces_y[i][j] = nullptr;
        }
    }
}

int Mesh::get_size_x() const {
    return m_size_x;
}

int Mesh::get_size_y() const {
    return m_size_y;
}

double Mesh::get_dx() const {
    return m_dx;
}

double Mesh::get_dy() const {
    return m_dy;
}

double Mesh::get_dt() const {
    return m_dt;
}

void Mesh::set_dt(const double dt) {
    m_dt = dt;
}

double Mesh::get_domain_size_x() const {
    return m_domain_size_x;
}

double Mesh::get_domain_size_y() const {
    return m_domain_size_y;
}

// Nodes

void Mesh::set_node(const int i, const int j, const double velocity_x, const double velocity_y, const double pressure, const double dye) {
    m_nodes[i][j] = std::make_unique<Node>(m_dx, m_dy, velocity_x, velocity_y, pressure, dye);
}

Node *Mesh::get_node(const int i, const int j) const {
    return m_nodes[i][j].get();
}

// Faces

void Mesh::set_interior_face(const FaceSide side, const int i, const int j) {
    if (side == FaceSide::X) {
        m_faces_x[i][j] = std::make_unique<InteriorFaceX>(m_dx, m_dy);
    } else {
        m_faces_y[i][j] = std::make_unique<InteriorFaceY>(m_dx, m_dy);
    }
}

void Mesh::set_boundary_inlet_face(const FaceSide side, const int i, const int j,
                                            const double velocity_x, const double velocity_y, const double dye) {
    if (side == FaceSide::X) {
        m_faces_x[i][j] = std::make_unique<InletBoundaryFace>(velocity_x, velocity_y, dye, Orientation::Horizontal);
    } else {
        m_faces_y[i][j] = std::make_unique<InletBoundaryFace>(velocity_x, velocity_y, dye, Orientation::Vertical);
    }
}

void Mesh::set_boundary_no_slip_face(const FaceSide side, const int i, const int j) {
    if (side == FaceSide::X) {
        m_faces_x[i][j] = std::make_unique<NoSlipBoundaryFace>(Orientation::Horizontal);
    } else {
        m_faces_y[i][j] = std::make_unique<NoSlipBoundaryFace>(Orientation::Vertical);
    }
}

void Mesh::set_boundary_moving_wall_face(const FaceSide side, const int i, const int j, const double velocity) {
    if (side == FaceSide::X) {
        m_faces_x[i][j] = std::make_unique<MovingWallBoundaryFace>(velocity, Orientation::Horizontal);
    } else {
        m_faces_y[i][j] = std::make_unique<MovingWallBoundaryFace>(velocity, Orientation::Vertical);
    }
}

void Mesh::set_boundary_slip_face(const FaceSide side, const int i, const int j) {
    if (side == FaceSide::X) {
        m_faces_x[i][j] = std::make_unique<SlipBoundaryFace>(Orientation::Horizontal);
    } else {
        m_faces_y[i][j] = std::make_unique<SlipBoundaryFace>(Orientation::Vertical);
    }
}

void Mesh::set_boundary_fixed_pressure_face(const FaceSide side, const int i, const int j, const double pressure) {
    if (side == FaceSide::X) {
        m_faces_x[i][j] = std::make_unique<FixedPressureBoundaryFace>(pressure, Orientation::Horizontal);
    } else {
        m_faces_y[i][j] = std::make_unique<FixedPressureBoundaryFace>(pressure, Orientation::Vertical);
    }
}

void Mesh::set_boundary_free_face(const FaceSide side, const int i, const int j) {
    if (side == FaceSide::X) {
        m_faces_x[i][j] = std::make_unique<FreeBoundaryFace>(Orientation::Horizontal);
    } else {
        m_faces_y[i][j] = std::make_unique<FreeBoundaryFace>(Orientation::Vertical);
    }
}

Face *Mesh::get_face_x(const int i, const int j) const {
    return m_faces_x[i][j].get();
}

Face *Mesh::get_face_y(const int i, const int j) const {
    return m_faces_y[i][j].get();
}

// Linking

void Mesh::link_nodes() const {
    for (int i = 0; i < get_size_x(); i++) {
        for (int j = 0; j < get_size_y(); j++) {
            Node *node = get_node(i, j);

            if (node == nullptr) {
                continue;
            }

            if (i != 0) {
                node->set_neighbouring_node(get_node(i - 1, j), Direction::West);
                if (i != 1) {
                    node->set_neighbouring_node(get_node(i - 2, j), Direction::WestWest);
                }
            }
            if (i != get_size_x() - 1) {
                node->set_neighbouring_node(get_node(i + 1, j), Direction::East);
                if (i != get_size_x() - 2) {
                    node->set_neighbouring_node(get_node(i + 2, j), Direction::EastEast);
                }
            }
            if (j != 0) {
                node->set_neighbouring_node(get_node(i, j - 1), Direction::South);
                if (j != 1) {
                    node->set_neighbouring_node(get_node(i, j - 2), Direction::SouthSouth);
                }
            }
            if (j != get_size_y() - 1) {
                node->set_neighbouring_node(get_node(i, j + 1), Direction::North);
                if (j != get_size_y() - 2) {
                    node->set_neighbouring_node(get_node(i, j + 2), Direction::NorthNorth);
                }
            }
        }
    }
}

void Mesh::link_nodes_faces() {
    for (int i = 0; i < get_size_x(); i++) {
        for (int j = 0; j < get_size_y(); j++) {
            Node *node = get_node(i, j);

            if (node == nullptr) {
                continue;
            }

            // West west
            if (node->get_neighbouring_face(Direction::WestWest) == nullptr && i != 0) {
                Face *face_ww = get_face_x(i - 1, j);
                if (face_ww == nullptr) {
                    if (node->get_neighbouring_node(Direction::WestWest) == nullptr) {
                        set_boundary_no_slip_face(FaceSide::X, i - 1, j);
                    } else {
                        set_interior_face(FaceSide::X, i - 1, j);
                    }
                    face_ww = get_face_x(i - 1, j);
                }
                node->set_neighbouring_face(face_ww, Direction::WestWest);
            }

            // West
            if (node->get_neighbouring_face(Direction::West) == nullptr) {
                Face *grid_face_w = get_face_x(i, j);
                if (grid_face_w == nullptr) {
                    if (node->get_neighbouring_node(Direction::West) == nullptr) {
                        set_boundary_no_slip_face(FaceSide::X, i, j);
                    } else {
                        set_interior_face(FaceSide::X, i, j);
                    }
                    grid_face_w = get_face_x(i, j);
                }
                node->set_neighbouring_face(grid_face_w, Direction::West);
                if (grid_face_w->get_face_type() == FaceType::Boundary) {
                    static_cast<BoundaryFace *>(grid_face_w)->set_node_neighbour(node);
                } else {
                    static_cast<InteriorFaceX *>(grid_face_w)->set_node_neighbour(node, FaceXSide::East);
                }
            }

            // East east
            if (node->get_neighbouring_face(Direction::EastEast) == nullptr && i != get_size_x() - 1) {
                Face *face_ee = get_face_x(i + 2, j);
                if (face_ee == nullptr) {
                    if (node->get_neighbouring_node(Direction::EastEast) == nullptr) {
                        set_boundary_no_slip_face(FaceSide::X, i + 2, j);
                    } else {
                        set_interior_face(FaceSide::X, i + 2, j);
                    }
                    face_ee = get_face_x(i + 2, j);
                }
                node->set_neighbouring_face(face_ee, Direction::EastEast);
            }

            // East
            if (node->get_neighbouring_face(Direction::East) == nullptr) {
                Face *grid_face_e = get_face_x(i + 1, j);
                if (grid_face_e == nullptr) {
                    if (node->get_neighbouring_node(Direction::East) == nullptr) {
                        set_boundary_no_slip_face(FaceSide::X, i + 1, j);
                    } else {
                        set_interior_face(FaceSide::X, i + 1, j);
                    }
                    grid_face_e = get_face_x(i + 1, j);
                }
                node->set_neighbouring_face(grid_face_e, Direction::East);
                if (grid_face_e->get_face_type() == FaceType::Boundary) {
                    static_cast<BoundaryFace *>(grid_face_e)->set_node_neighbour(node);
                } else {
                    static_cast<InteriorFaceX *>(grid_face_e)->set_node_neighbour(node, FaceXSide::West);
                }
            }

            // South south
            if (node->get_neighbouring_face(Direction::SouthSouth) == nullptr && j != 0) {
                Face *face_ss = get_face_y(i, j - 1);
                if (face_ss == nullptr) {
                    if (node->get_neighbouring_node(Direction::SouthSouth) == nullptr) {
                        set_boundary_no_slip_face(FaceSide::Y, i, j - 1);
                    } else {
                        set_interior_face(FaceSide::Y, i, j - 1);
                    }
                    face_ss = get_face_y(i, j - 1);
                }
                node->set_neighbouring_face(face_ss, Direction::SouthSouth);
            }

            // South
            if (node->get_neighbouring_face(Direction::South) == nullptr) {
                Face *grid_face_s = get_face_y(i, j);
                if (grid_face_s == nullptr) {
                    if (node->get_neighbouring_node(Direction::South) == nullptr) {
                        set_boundary_no_slip_face(FaceSide::Y, i, j);
                    } else {
                        set_interior_face(FaceSide::Y, i, j);
                    }
                    grid_face_s = get_face_y(i, j);
                }
                node->set_neighbouring_face(grid_face_s, Direction::South);
                if (grid_face_s->get_face_type() == FaceType::Boundary) {
                    static_cast<BoundaryFace *>(grid_face_s)->set_node_neighbour(node);
                } else {
                    static_cast<InteriorFaceY *>(grid_face_s)->set_node_neighbour(node, FaceYSide::North);
                }
            }

            // North north
            if (node->get_neighbouring_face(Direction::NorthNorth) == nullptr && j != get_size_y() - 1) {
                Face *face_nn = get_face_y(i, j + 2);
                if (face_nn == nullptr) {
                    if (node->get_neighbouring_node(Direction::NorthNorth) == nullptr) {
                        set_boundary_no_slip_face(FaceSide::Y, i, j + 2);
                    } else {
                        set_interior_face(FaceSide::Y, i, j + 2);
                    }
                    face_nn = get_face_y(i, j + 2);
                }
                node->set_neighbouring_face(face_nn, Direction::NorthNorth);
            }

            // North
            if (node->get_neighbouring_face(Direction::North) == nullptr) {
                Face *grid_face_n = get_face_y(i, j + 1);
                if (grid_face_n == nullptr) {
                    if (node->get_neighbouring_node(Direction::North) == nullptr) {
                        set_boundary_no_slip_face(FaceSide::Y, i, j + 1);
                    } else {
                        set_interior_face(FaceSide::Y, i, j + 1);
                    }
                    grid_face_n = get_face_y(i, j + 1);
                }
                node->set_neighbouring_face(grid_face_n, Direction::North);
                if (grid_face_n->get_face_type() == FaceType::Boundary) {
                    static_cast<BoundaryFace *>(grid_face_n)->set_node_neighbour(node);
                } else {
                    static_cast<InteriorFaceY *>(grid_face_n)->set_node_neighbour(node, FaceYSide::South);
                }
            }
        }
    }
}
