#ifndef SOLVER
#define SOLVER

#include <set>
#include <stack>
#include "Grid.h"
#include "View.h"
#include "Coordinates.h"
#include "Checkpoint.h"

class Solver{
    private:
        Grid& grid;
        View& v;
        std::stack<Checkpoint> checkpoints;
        std::set<Coordinates> attempted_coords;
        std::set<unsigned> attempted_vals;
        Coordinates* selected_cell;
        bool checkpoint_restored;

        void restore_last_checkpoint();

    public:
        Solver(Grid& grid, View& view);

        Coordinates select_cell();
        unsigned select_val(Coordinates c);
        void solve();
};

#endif // SOLVER