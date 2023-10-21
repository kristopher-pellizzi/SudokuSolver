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
        Coordinates(const Coordinates& c);

        unsigned get_x() const;
        unsigned get_y() const;
        std::string to_string() const;

        bool operator<(const Coordinates& other) const;
        bool operator==(const Coordinates& other) const;
};

#endif // COORDS