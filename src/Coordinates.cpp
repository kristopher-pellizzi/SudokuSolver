#include "Coordinates.h"
#include <sstream>

Coordinates::Coordinates() : x(0), y(0) {}

Coordinates::Coordinates(unsigned x, unsigned y) : x(x), y(y) {}

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