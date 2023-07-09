#include "Grid.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;

Grid::Grid(unsigned block_width) : block_width(block_width), grid_width(block_width * block_width) {
    unsigned total_size = grid_width * grid_width;

    grid = (unsigned*) calloc(total_size, sizeof(unsigned));
}

unsigned Grid::get_block_width(){
    return block_width;
}

unsigned Grid::get_grid_width(){
    return grid_width;
}

// Return 0 if OK; -1 if KO
int Grid::check_coordinates(unsigned x, unsigned y) const{
    if(x >= grid_width|| y >= grid_width){
        cout << "Cell coordinates must be integers from 0 to " << grid_width - 1 << endl;
        return -1;
    }

    return 0;
}

unsigned Grid::get(unsigned x, unsigned y) const{    
    if (check_coordinates(x, y) == -1)
        return 0;

    return grid[x * grid_width + y];
}

void Grid::set(unsigned x, unsigned y, unsigned val){
    if (check_coordinates(x, y) == -1)
        return;

    grid[x * grid_width + y] = val;
}

void Grid::init(){
    std::ifstream init_file("init.txt");

    for (int i = 0; i < grid_width; ++i){
        for (int j = 0; j < grid_width; ++j){
            unsigned val;
            init_file >> val;
            if (val > 0 && val <= grid_width)
                set(i, j, val);
        }
    }

    init_file.close();
}

unsigned* Grid::operator[](unsigned idx) const{
    return grid + idx * grid_width;
}