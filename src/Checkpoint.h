#ifndef CHECKPOINT
#define CHECKPOINT

#include <set>
#include "Grid.h"
#include "Coordinates.h"
#include "LockedCandidateIndex.h"

class Checkpoint{
    friend class Solver;

    private:
        unsigned filled_cells;
        Cell* grid;
        std::set<Coordinates> attempted_coords;
        std::set<unsigned> attempted_vals;
        std::set<LockedCandidateIndex> row_locked_candidates;
        std::set<LockedCandidateIndex> col_locked_candidates;
        Coordinates* selected_coord;

        Checkpoint(Grid& grid);

    public:
        ~Checkpoint();
        void restore(Grid& grid);
};

#endif // CHECKPOINT