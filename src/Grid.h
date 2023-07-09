#ifndef GRID
#define GRID

#include "Cell.h"

class Grid {
    private:
        const unsigned block_width;
        const unsigned grid_width;
        Cell* grid;

        int check_coordinates(unsigned x, unsigned y) const;

    public:
        Grid(unsigned block_width);

        unsigned get_block_width();
        unsigned get_grid_width();
        unsigned get(unsigned x, unsigned y) const;
        void set(unsigned x, unsigned y, unsigned val);
        void init();

        Cell* operator[](unsigned idx) const;
};

#endif //GRID