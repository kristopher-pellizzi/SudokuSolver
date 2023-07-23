#ifndef CHECKPOINT
#define CHECKPOINT

#include <set>
#include "Grid.h"
#include "Coordinates.h"

class Checkpoint{
    friend class Solver;

    private:
        unsigned filled_cells;
        Cell* grid;
        std::set<Coordinates> attempted_coords;
        std::set<unsigned> attempted_vals;
        Coordinates* selected_coord;

        Checkpoint(Grid& grid);

    public:
        ~Checkpoint();
        void restore(Grid& grid);
};

#endif // CHECKPOINT