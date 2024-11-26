#pragma once

#include "../../../../common.hpp"

enum class CoefficientType { Center = 0, Source = 1, West = 2, East = 3, South = 4, North = 5, WestWest = 6, EastEast = 7, SouthSouth = 8, NorthNorth = 9 };

class Coefficients {
public:
    double center = 0.0;
    double source = 0.0;
    double west = 0.0;
    double east = 0.0;
    double south = 0.0;
    double north = 0.0;
    double west_west = 0.0;
    double east_east = 0.0;
    double south_south = 0.0;
    double north_north = 0.0;

    double get_coefficient(CoefficientType type) const;

    double get_coefficient(Direction direction) const;

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
