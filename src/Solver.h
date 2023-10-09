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
        std::stack<Checkpoint*> checkpoints;
        std::set<Coordinates> attempted_coords;
        std::set<unsigned> attempted_vals;
        Coordinates* selected_cell;
        bool checkpoint_restored;

        Coordinates* do_search_hidden_single(const Coordinates& c, std::set<Coordinates>& adjacent_cells);
        Coordinates* search_row_hidden_single(const Coordinates& c);
        Coordinates* search_col_hidden_single(const Coordinates& c);
        Coordinates* search_block_hidden_single(const Coordinates& c);
        Coordinates* search_hidden_single();
        Coordinates select_cell();
        unsigned select_val(Coordinates& c);
        void restore_last_checkpoint();
        void clean();

    public:
        Solver(Grid& grid, View& view);

        
        void solve();
};

#endif // SOLVER