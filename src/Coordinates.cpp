#include "Coordinates.h"
#include <sstream>

Coordinates::Coordinates() : x(0), y(0) {}

Coordinates::Coordinates(unsigned x, unsigned y) : x(x), y(y) {}

Coordinates::Coordinates(const Coordinates& c): x(c.x), y(c.y) {}

unsigned Coordinates::get_x() const{
    return x;
}

unsigned Coordinates::get_y() const{
    return y;
}

std::string Coordinates::to_string() const{
    std::stringstream sstream;

    sstream << "(" << x << ", " << y << ")";
    return sstream.str();
}

bool Coordinates::operator<(const Coordinates& other) const{
    if(x != other.x)
        return x < other.x;

    return y < other.y;
}

bool Coordinates::operator==(const Coordinates& other) const{
    return (x == other.x && y == other.y);
}