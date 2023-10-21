#ifndef GRID
#define GRID

#include <set>
#include "Cell.h"
#include "Coordinates.h"
#include "View.h"

class Grid {
    friend class Checkpoint;

    private:
        const unsigned block_width;
        const unsigned grid_width;
        unsigned filled_cells;
        bool constraints_initialized;
        Cell* grid;
        View* v;

        int check_coordinates(unsigned x, unsigned y) const;
        void constrain_line(unsigned x, unsigned val);
        void constrain_column(unsigned y, unsigned val);
        void constrain_block(unsigned x, unsigned y, unsigned val);
        void add_constraint(unsigned x, unsigned y, unsigned val);

        // Simulate add constraints methods
        unsigned simulate_constrain_line(unsigned x, unsigned val);
        unsigned simulate_constrain_column(unsigned y, unsigned val);
        unsigned simulate_constrain_block(unsigned x, unsigned y, unsigned val); 

    public:
        Grid(unsigned block_width);

        void set_view(View& v);
        unsigned get_block_width();
        unsigned get_grid_width();
        unsigned get(unsigned x, unsigned y) const;
        bool is_filled(unsigned x, unsigned y) const;
        bool is_filled(Coordinates& c) const;
        unsigned get_filled_cells() const;
        void set(unsigned x, unsigned y, unsigned val);
        unsigned get(Coordinates& c) const;
        void set(Coordinates& c, unsigned val);
        void init(string& grid_path);
        void init_constraints();
        std::set<unsigned>& get_available_vals(unsigned x, unsigned y);
        std::set<unsigned>& get_available_vals(const Coordinates& c);
        bool is_completed() const;
        unsigned simulate_add_constraint(unsigned x, unsigned y, unsigned val);
        std::set<Coordinates> get_row_adjacent_cells(const Coordinates& c) const;
        std::set<Coordinates> get_col_adjacent_cells(const Coordinates& c) const;
        std::set<Coordinates> get_block_adjacent_cells(const Coordinates& c) const;
        void check_initial_constraints() const;
        void set_hidden_single(const Coordinates& c, unsigned val);
        void set_row_locked_candidates(unsigned block_idx, unsigned block_row_idx, const std::set<unsigned>& vals);
        void set_col_locked_candidates(unsigned block_idx, unsigned block_col_idx, const std::set<unsigned>& vals);

        Cell* operator[](unsigned idx) const;
};

#endif //GRID