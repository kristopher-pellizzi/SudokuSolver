#include <sstream>
#include <random>
#include "RandomGridInitializer.h"
#include "Coordinates.h"
#include "Solver.h"

RandomGridInitializer::RandomGridInitializer(View* v) : v(v){}

void RandomGridInitializer::init_block(RandomNumberGenerator& rng, unsigned grid_width, Grid& grid, unsigned i, unsigned j){
    unsigned val = 0;
    std::set<unsigned> available_vals = grid.get_available_vals(i, j);

    if (available_vals.size() == 0){
        std::stringstream sstream;
        sstream << "No available values left for cell (" << i << ", " << j << ")." << std::endl;
        sstream << "Program aborted" << std::endl;
        v->error(sstream.str());
    }

    unsigned rand_idx = rng.randInt(0, available_vals.size() - 1);
    unsigned idx = 0;
    auto iter = available_vals.begin();

    while (idx++ != rand_idx){
        ++iter;
    }

    val = *iter;
    if (val < 0 || val > grid_width){
        std::stringstream sstream;
        sstream << "Invalid value " << val << "." << std::endl;
        sstream << "Check initialization values.";
        v->error(sstream.str());
    }

    if (val != 0)
        grid.set(i, j, val);
}

void RandomGridInitializer::init(Grid& grid){
    unsigned block_width = grid.get_block_width();
    unsigned grid_width = block_width * block_width;
    RandomNumberGenerator rng;
    unsigned total_cells = grid_width * grid_width;
    unsigned hints = rng.randInt(17, 30);
    unsigned to_remove = total_cells - hints;
    Grid solution(grid);

    v->message("Initializing grid...");

    // Initialize diagonal blocks
    for(unsigned start_idx = 0; start_idx < grid_width; start_idx += block_width){
        unsigned end_idx = start_idx + block_width;

        for(unsigned i = start_idx; i < end_idx; ++i){
            for(unsigned j = start_idx; j < end_idx; ++j){
                init_block(rng, grid_width, solution, i, j);
            }
        }
    }

    Solver solver(solution, *v, true);
    solver.solve();

    std::set<Coordinates> empty_coords;
    while (empty_coords.size() < to_remove){
        unsigned x = rng.randInt(0, grid_width - 1);
        unsigned y = rng.randInt(0, grid_width - 1);

        empty_coords.insert(Coordinates(x, y));
    }

    for (unsigned x = 0; x < grid_width; ++x){
        for (unsigned y = 0; y < grid_width; ++y){
            Coordinates c(x, y);

            if (!grid.is_filled(c) && empty_coords.find(c) == empty_coords.end()){
                unsigned sol_val = solution.get(x, y);
                grid.set(x, y, sol_val);
            }
        }
    }

    grid.check_initial_constraints();
}