#include "Grid.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

Grid::Grid(unsigned block_width) : 
    block_width(block_width), 
    grid_width(block_width * block_width),
    filled_cells(0)
{
    unsigned total_size = grid_width * grid_width;

    grid = (Cell*) calloc(total_size, sizeof(Cell));
    
    for(unsigned i = 0; i < total_size; ++i){
        grid[i] = Cell(0, grid_width);
    }
}

Grid::Grid(const Grid& other) :
    block_width(other.block_width),
    grid_width(other.grid_width),
    filled_cells(other.filled_cells)
{
    unsigned total_size = grid_width * grid_width;

    grid = (Cell*) calloc(total_size, sizeof(Cell));
    
    for(unsigned i = 0; i < total_size; ++i){
        grid[i] = Cell(other.grid[i]);
    }
}

Grid& Grid::operator=(const Grid& other){
    filled_cells = other.filled_cells;
    unsigned total_size = grid_width * grid_width;

    grid = (Cell*) calloc(total_size, sizeof(Cell));
    
    for(unsigned i = 0; i < total_size; ++i){
        grid[i] = Cell(other.grid[i]);
    }

    return *this;
}

Grid::~Grid(){
    free(grid);
}

void Grid::set_view(View& view){
    v = &view;
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
        std::stringstream sstream;
        sstream << "Invalid coordinates (" << x << ", " << y << ")" << std::endl;
        sstream << "Cell coordinates must be integers from 0 to " << grid_width - 1; 
        v->warning(sstream.str());
        return -1;
    }

    return 0;
}

unsigned Grid::get(unsigned x, unsigned y) const{    
    if (check_coordinates(x, y) == -1)
        return 0;

    return grid[x * grid_width + y].get();
}

unsigned Grid::get(Coordinates& c) const{
    return get(c.get_x(), c.get_y());
}

bool Grid::is_filled(unsigned x, unsigned y) const{
    return get(x, y) != 0;
}

bool Grid::is_filled(Coordinates& c) const{
    return is_filled(c.get_x(), c.get_y());
}

unsigned Grid::get_filled_cells() const{
    return filled_cells;
}

void Grid::constrain_line(unsigned x, unsigned val){
    unsigned row_idx = x;

    for (unsigned col_idx = 0; col_idx < grid_width; ++col_idx){
        grid[row_idx * grid_width + col_idx].constrain(val);
    }
}

void Grid::constrain_column(unsigned y, unsigned val){
    for (unsigned idx = 0; idx < grid_width; ++idx){
        grid[idx * grid_width + y].constrain(val);
    }
}

void Grid::constrain_block(unsigned x, unsigned y, unsigned val){
    unsigned block_row_idx = x / block_width;
    unsigned block_col_idx = y / block_width;
    unsigned row_idx_init = block_row_idx * block_width;
    unsigned row_idx_limit = row_idx_init + block_width;
    unsigned col_idx_init = block_col_idx * block_width;
    unsigned col_idx_limit = col_idx_init + block_width;

    for (unsigned row_idx = row_idx_init; row_idx < row_idx_limit; ++row_idx){
        for (unsigned col_idx = col_idx_init; col_idx < col_idx_limit; ++col_idx){
            grid[row_idx * grid_width + col_idx].constrain(val);
        }
    }
}

void Grid::add_constraint(unsigned x, unsigned y, unsigned val){
    constrain_line(x, val);
    constrain_column(y, val);
    constrain_block(x, y, val);
}

void Grid::set(unsigned x, unsigned y, unsigned val){
    if (check_coordinates(x, y) == -1)
        return;

    if(val < 1 || val > grid_width){
        std::stringstream sstream;
        sstream << "Invalid value " << val << "." << std::endl;
        sstream << "Values must be integers between 1 and " << grid_width;
        v->warning(sstream.str());
        return;
    }

    unsigned index = x * grid_width + y;
    int ret = grid[index].set(val);

    // Check if the set has been successful
    if (ret != 0){
        std::stringstream sstream;
        sstream << "Value " << val << " cannot be set in cell (" << x << ", " << y << ")";
        v->warning(sstream.str());
        return;
    }

    add_constraint(x, y, val);
    ++filled_cells;
}

void Grid::set(Coordinates& c, unsigned val){
    set(c.get_x(), c.get_y(), val);
}

std::set<unsigned>& Grid::get_available_vals(unsigned x, unsigned y){
    return grid[x * grid_width + y].get_available_vals();
}

std::set<unsigned>& Grid::get_available_vals(const Coordinates& c){
    return grid[c.get_x() * grid_width + c.get_y()].get_available_vals();
}

bool Grid::is_completed() const {
    return filled_cells == grid_width * grid_width;
}

Cell* Grid::operator[](unsigned idx) const{
    return grid + idx * grid_width;
}


// Simulate add constraints methods that return the smallest number of available values 
// For each of the constrained cells
unsigned Grid::simulate_constrain_line(unsigned x, unsigned val){
    unsigned row_idx = x;
    unsigned min = UINT_MAX;

    for (unsigned col_idx = 0; col_idx < grid_width; ++col_idx){
        std::set<unsigned>& available_vals = grid[row_idx * grid_width + col_idx].get_available_vals();

        unsigned num_vals = is_filled(row_idx, col_idx) ? 1 : available_vals.size();

        if (available_vals.find(val) != available_vals.end()){
            --num_vals;
        }

        if (num_vals < min)
            min = num_vals;
    }

    return min;
}

unsigned Grid::simulate_constrain_column(unsigned y, unsigned val){
    unsigned min = UINT_MAX;

    for (unsigned idx = 0; idx < grid_width; ++idx){
        std::set<unsigned>& available_vals = grid[idx * grid_width + y].get_available_vals();
        unsigned num_vals = is_filled(idx, y) ? 1 : available_vals.size();

        if (available_vals.find(val) != available_vals.end()){
            --num_vals;
        }

        if (num_vals < min)
            min = num_vals;
    }

    return min;
}

unsigned Grid::simulate_constrain_block(unsigned x, unsigned y, unsigned val){
    unsigned block_row_idx = x / block_width;
    unsigned block_col_idx = y / block_width;
    unsigned row_idx_init = block_row_idx * block_width;
    unsigned row_idx_limit = row_idx_init + block_width;
    unsigned col_idx_init = block_col_idx * block_width;
    unsigned col_idx_limit = col_idx_init + block_width;
    unsigned min = UINT_MAX;

    for (unsigned row_idx = row_idx_init; row_idx < row_idx_limit; ++row_idx){
        for (unsigned col_idx = col_idx_init; col_idx < col_idx_limit; ++col_idx){
            std::set<unsigned>& available_vals = grid[row_idx * grid_width + col_idx].get_available_vals();
            unsigned num_vals = is_filled(row_idx, col_idx) ? 1 : available_vals.size();

            if (available_vals.find(val) != available_vals.end()){
                --num_vals;
            }

            if (num_vals < min)
                min = num_vals;
        }
    }

    return min;
}

unsigned Grid::simulate_add_constraint(unsigned x, unsigned y, unsigned val){
    unsigned line = simulate_constrain_line(x, val);
    unsigned column = simulate_constrain_column(y, val);
    unsigned block = simulate_constrain_block(x, y, val);

    return std::min(std::min(line, column), block);
}

std::set<Coordinates> Grid::get_row_adjacent_cells(const Coordinates& c) const{
    unsigned c_x = c.get_x();
    unsigned c_y = c.get_y();
    std::set<Coordinates> ret;

    for (unsigned col_idx = 0; col_idx < grid_width; ++col_idx){
        if(col_idx != c_y)
            ret.insert(Coordinates(c_x, col_idx));
    }

    return ret;
}

std::set<Coordinates> Grid::get_col_adjacent_cells(const Coordinates& c) const{
    unsigned c_x = c.get_x();
    unsigned c_y = c.get_y();
    std::set<Coordinates> ret;

    for (unsigned idx = 0; idx < grid_width; ++idx){
        if (idx != c_x)
            ret.insert(Coordinates(idx, c_y));
    }

    return ret;
}

std::set<Coordinates> Grid::get_block_adjacent_cells(const Coordinates& c) const{
    std::set<Coordinates> ret;
    unsigned c_x = c.get_x();
    unsigned c_y = c.get_y();

    unsigned block_row_idx = c_x / block_width;
    unsigned block_col_idx = c_y / block_width;
    unsigned row_idx_init = block_row_idx * block_width;
    unsigned row_idx_limit = row_idx_init + block_width;
    unsigned col_idx_init = block_col_idx * block_width;
    unsigned col_idx_limit = col_idx_init + block_width;

    for (unsigned row_idx = row_idx_init; row_idx < row_idx_limit; ++row_idx){
        for (unsigned col_idx = col_idx_init; col_idx < col_idx_limit; ++col_idx){
            if (row_idx != c_x || col_idx != c_y){
                ret.insert(Coordinates(row_idx, col_idx));
            }
        }
    }

    return ret;
}

void Grid::check_initial_constraints() const{
    unsigned grid_width = block_width * block_width;
    
    for(unsigned i = 0; i < grid_width; ++i){
        for(unsigned j = 0; j < grid_width; ++j){
            Coordinates c(i, j);
            unsigned val = get(c.get_x(), c.get_y());
            if (val == 0)
                continue;

            std::set<Coordinates> adjacent_cells = get_row_adjacent_cells(c);
            std::set<Coordinates> col = get_col_adjacent_cells(c);
            std::set<Coordinates> block = get_block_adjacent_cells(c);
            adjacent_cells.insert(col.begin(), col.end());
            adjacent_cells.insert(block.begin(), block.end());

            for(auto iter = adjacent_cells.begin(); iter != adjacent_cells.end(); ++iter){
                unsigned iter_val = get(iter->get_x(), iter->get_y());

                if (iter_val == val){
                    std::stringstream sstream;
                    sstream << "Initial state of the grid is infeasible:" << std::endl;
                    sstream << "Cell " << iter->to_string() << " has the same value (" << val << ") as cell " << c.to_string() << std::endl;
                    v->error(sstream.str());
                }
            }
        }
    }
}

void Grid::set_hidden_single(const Coordinates& c, unsigned val){
    grid[c.get_x() * grid_width + c.get_y()].set_hidden_single(val);
}

void Grid::set_row_locked_candidates(unsigned block_idx, unsigned block_row_idx, const std::set<unsigned>& vals){
    unsigned row_idx = block_idx / block_width * block_width + block_row_idx;
    unsigned col_idx = (block_idx % block_width) * block_width;
    unsigned grid_start_idx = row_idx * grid_width;

    for(unsigned i = 0; i < grid_width; ++i){
        // Do not remove vals from available_vals of the block where the locked candidate has been found
        if (i >= col_idx && i < col_idx + block_width)
            continue;
            
        unsigned idx = grid_start_idx + i;
        for(auto iter = vals.begin(); iter != vals.end(); ++iter){
            grid[idx].constrain(*iter);
        }
    }
}

void Grid::set_col_locked_candidates(unsigned block_idx, unsigned block_col_idx, const std::set<unsigned>& vals){
    unsigned row_idx = block_idx / block_width * block_width;
    unsigned col_idx = (block_idx % block_width) * block_width + block_col_idx;

    for(unsigned i = 0; i < grid_width; ++i){
        if (i >= row_idx && i < row_idx + block_width)
            continue;

        unsigned idx = i * grid_width + col_idx;

        for(auto iter = vals.begin(); iter != vals.end(); ++iter){
            grid[idx].constrain(*iter);
        }
    }
}