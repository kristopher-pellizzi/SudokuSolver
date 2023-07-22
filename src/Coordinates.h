#ifndef COORDS
#define COORDS

#include <string>

class Coordinates{
    private:
        unsigned x;
        unsigned y;

    public:
        Coordinates();
        Coordinates(unsigned x, unsigned y);

        unsigned get_x() const;
        unsigned get_y() const;
        std::string to_string() const;
};

#endif // COORDS