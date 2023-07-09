#include "Grid.h"
#include "CliView.h"

int main(){
    unsigned block_width = 3;
    Grid grid(block_width);    
    grid.init();
    CliView v(grid);
    v.draw();

    return 0;
}