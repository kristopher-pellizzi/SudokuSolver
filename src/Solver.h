#ifndef SOLVER
#define SOLVER

#include <set>
#include <stack>
#include "Grid.h"
#include "View.h"
#include "Coordinates.h"
#include "Checkpoint.h"
#include "LockedCandidateIndex.h"

class Solver{
    private:
        Grid& grid;
        View& v;
        std::stack<Checkpoint*> checkpoints;
        std::set<Coordinates> attempted_coords;
        std::set<unsigned> attempted_vals;
        std::set<LockedCandidateIndex> row_locked_candidates;
        std::set<LockedCandidateIndex> col_locked_candidates;
        Coordinates* selected_cell;
        bool candidate_locked;
        bool checkpoint_restored;

        Coordinates* do_search_hidden_single(const Coordinates& c, std::set<Coordinates>& adjacent_cells);
        Coordinates* search_row_hidden_single(const Coordinates& c);
        Coordinates* search_col_hidden_single(const Coordinates& c);
        Coordinates* search_block_hidden_single(const Coordinates& c);
        Coordinates* search_hidden_single();
        bool try_lock_row_candidates(unsigned block_idx, unsigned first_row_idx, const std::set<Coordinates>& block_cells);
        bool try_lock_col_candidates(unsigned block_idx, unsigned first_col_idx, const std::set<Coordinates>& block_cells);
        bool try_lock_candidates();
        Coordinates select_cell();
        unsigned select_val(Coordinates& c);
        void restore_last_checkpoint();
        void clean();

    public:
        Solver(Grid& grid, View& view);

        
        void solve();
};

#endif // SOLVER