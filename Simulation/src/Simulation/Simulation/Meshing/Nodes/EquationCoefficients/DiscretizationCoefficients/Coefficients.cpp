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

void Coefficients::set_coefficient(const Direction direction, const double value) {
    switch (direction) {
        case Direction::West: {
            west = value;
            break;
        }
        case Direction::East: {
            east = value;
            break;
        }
        case Direction::South: {
            south = value;
            break;
        }
        case Direction::North: {
            north = value;
            break;
        }
        case Direction::WestWest: {
            west_west = value;
            break;
        }
        case Direction::EastEast: {
            east_east = value;
            break;
        }
        case Direction::SouthSouth: {
            south_south = value;
            break;
        }
        case Direction::NorthNorth: {
            north_north = value;
            break;
        }
    }
}

void Coefficients::add_to_coefficient(const Direction direction, const double value) {
    set_coefficient(direction, get_coefficient(direction) + value);
}
