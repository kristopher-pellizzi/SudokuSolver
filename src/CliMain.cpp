#include "CliView.h"
#include "Grid.h"
#include "Solver.h"

int main(){
    unsigned block_width = 3;
    Grid grid(block_width);    
    CliView v(grid);
    grid.set_view(v);
    grid.init();
    v.draw();

    Solver solver(grid, v);
    solver.solve();
    return 0;
}