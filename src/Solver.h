#ifndef SOLVER
#define SOLVER

#include "Grid.h"
#include "View.h"
#include "Coordinates.h"

class Solver{
    private:
        Grid& grid;
        View& v;

    public:
        Solver(Grid& grid, View& view);

        Coordinates select_cell() const;
        unsigned select_val(Coordinates c) const;
        void solve() const;
};

#endif // SOLVER