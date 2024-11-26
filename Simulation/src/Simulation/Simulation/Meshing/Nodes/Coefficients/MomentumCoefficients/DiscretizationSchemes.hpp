#pragma once

enum class Scheme { Upwind, CentralDifferencing, Hybrid };

struct SchemeCoefficients {
    double node0 = 0.0;
    double node1 = 0.0;
    double node2 = 0.0;
    double node3 = 0.0;
    double source = 0.0;

    SchemeCoefficients operator+(const SchemeCoefficients &rhs) const {
        return {
            .node0 = node0 + rhs.node0,
            .node1 = node1 + rhs.node1,
            .node2 = node2 + rhs.node2,
            .node3 = node3 + rhs.node3,
            .source = source + rhs.source
        };
    }

    SchemeCoefficients &operator+=(const SchemeCoefficients &rhs) {
        node0 += rhs.node0;
        node1 += rhs.node1;
        node2 += rhs.node2;
        node3 += rhs.node3;
        source += rhs.source;
        return *this;
    }
};

class Node;
enum class Direction;
enum class VelocityComponent;


class DiscretizationSchemes {
private:
    Node *m_node;

public:
    DiscretizationSchemes(Node *node);

    SchemeCoefficients convection(Direction direction, Scheme scheme, VelocityComponent velocity_component);

    SchemeCoefficients upwind(double flow_rate);

    SchemeCoefficients central_differencing(double flow_rate);

    SchemeCoefficients hybrid(double length, double velocity, double viscosity, double density, double flow_rate);
};
