#include "CliView.h"
#include "Grid.h"
#include "Solver.h"

int main(){
    unsigned block_width = 3;
    Grid grid(block_width);    
    grid.init();
    CliView v(grid);
    grid.set_view(v);
    v.draw();

    Solver solver(grid, v);
    solver.solve();
    return 0;
}