#include "Coefficients.hpp"

double Coefficients::get_coefficient(const CoefficientType type) const {
    switch (type) {
        case CoefficientType::Center:
            return center;
        case CoefficientType::Source:
            return source;
        case CoefficientType::West:
            return west;
        case CoefficientType::East:
            return east;
        case CoefficientType::South:
            return south;
        case CoefficientType::North:
            return north;
        case CoefficientType::WestWest:
            return west_west;
        case CoefficientType::EastEast:
            return east_east;
        case CoefficientType::SouthSouth:
            return south_south;
        case CoefficientType::NorthNorth:
            return north_north;
    }
}

double Coefficients::get_coefficient(const Direction direction) const {
    switch (direction) {
        case Direction::West:
            return west;
        case Direction::East:
            return east;
        case Direction::South:
            return south;
        case Direction::North:
            return north;
        case Direction::WestWest:
            return west_west;
        case Direction::EastEast:
            return east_east;
        case Direction::SouthSouth:
            return south_south;
        case Direction::NorthNorth:
            return north_north;
    }
}
