#ifndef IGRID
#define IGRID

#include <set>
#include "Coordinates.h"

class IGrid{
    public:
        virtual unsigned get(unsigned x, unsigned y) const = 0;
        virtual std::set<unsigned>& get_available_vals(unsigned x, unsigned y) = 0;
        virtual std::set<Coordinates>& get_fixed_cells() = 0;
};

#endif // IGRID