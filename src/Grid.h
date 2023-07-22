#ifndef GRID
#define GRID

#include <set>
#include "Cell.h"
#include "Coordinates.h"
#include "View.h"

class Grid {
    private:
        const unsigned block_width;
        const unsigned grid_width;
        unsigned filled_cells;
        Cell* grid;
        View* v;

        int check_coordinates(unsigned x, unsigned y) const;
        void constrain_line(unsigned x, unsigned val);
        void constrain_column(unsigned y, unsigned val);
        void constrain_block(unsigned x, unsigned y, unsigned val);
        void add_constraint(unsigned x, unsigned y, unsigned val);
        void init_constraints();

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
        void set(unsigned x, unsigned y, unsigned val);
        unsigned get(Coordinates c) const;
        void set(Coordinates c, unsigned val);
        void init();
        std::set<unsigned>& get_available_vals(unsigned x, unsigned y);
        std::set<unsigned>& get_available_vals(Coordinates c);
        bool is_completed() const;
        unsigned simulate_add_constraint(unsigned x, unsigned y, unsigned val);

        Cell* operator[](unsigned idx) const;
};

#endif //GRID