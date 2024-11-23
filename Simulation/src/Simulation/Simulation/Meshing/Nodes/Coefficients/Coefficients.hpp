#pragma once

#include "../../../../common.hpp"

enum class CoefficientType { Center = 0, Source = 1, West = 2, East = 3, South = 4, North = 5 };

class Coefficients {
public:
    double center = 0.0;
    double source = 0.0;
    double west = 0.0;
    double east = 0.0;
    double south = 0.0;
    double north = 0.0;

    double get_coefficient(CoefficientType type) const;

    Coefficients operator+(const Coefficients &rhs) const {
        return {center + rhs.center, source + rhs.source, west + rhs.west, east + rhs.east, south + rhs.south,
                north + rhs.north};
    }

    Coefficients &operator+=(const Coefficients &rhs) {
        center += rhs.center;
        source += rhs.source;
        west += rhs.west;
        east += rhs.east;
        south += rhs.south;
        north += rhs.north;
        return *this;
    }
};
