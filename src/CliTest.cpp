#include "Grid.h"
#include "CliView.h"

int main(){
    unsigned block_width = 3;
    Grid grid(block_width);    
    CliView v(grid);
    v.draw(&grid);

    unsigned grid_width = block_width * block_width;
    for (int i = 0; i < grid_width; i++){
        for (int j = 0; j < grid_width; j++){
            grid[i][j] = j + 1;
        }
    }

    v.draw(&grid);
    
    return 0;
}