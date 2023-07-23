#include "Checkpoint.h"

Checkpoint::Checkpoint(Grid& grid) :
    filled_cells(grid.filled_cells),
    selected_coord(NULL)
{
    unsigned grid_width = grid.grid_width;
    unsigned size = grid_width * grid_width;
    this->grid = (Cell*) calloc(size, sizeof(Cell));
    memcpy(this->grid, grid.grid, size);
}

Checkpoint::~Checkpoint(){
    if(selected_coord != NULL)
        delete selected_coord;
}

void Checkpoint::restore(Grid& grid){
    grid.filled_cells = filled_cells;
    unsigned size = grid.grid_width * grid.grid_width;
    memcpy(grid.grid, this->grid, size);
}