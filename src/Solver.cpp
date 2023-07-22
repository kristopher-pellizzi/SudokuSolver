#include <set>
#include <sstream>
#include <iostream>
#include "CliView.h"
#include "Solver.h"

Solver::Solver(Grid& grid, View& view) : grid(grid), v(view) {}

Coordinates Solver::select_cell() const {
    unsigned grid_width = grid.get_grid_width();
    Coordinates selected;
    unsigned min_available_vals = UINT_MAX;

    for (unsigned i = 0; i < grid_width; ++i){
        for (unsigned j = 0; j < grid_width; ++j){
            // If cell already has a value, ignore it
            if (grid[i][j] != 0)
                continue;

            std::set<unsigned>& available_vals = grid.get_available_vals(i, j);

            // TODO: check if there's any empty cell with no more available values
            // If it is found, pop the last checkpoint and restore this method from the 
            // coord following the stored one.
            // If there are no more, pop another checkpoint and so on

            // Try to find the most constrained cell
            if (available_vals.size() < min_available_vals){
                min_available_vals = available_vals.size();
                selected = Coordinates(i, j);
            }
        }
    }

    // TODO: If available vals size is > 1, create and push a new checkpoint. Store in the checkpoint the current coords
    std::stringstream sstream;
    sstream << "Selected most constrained cell (" << min_available_vals << " constraints) with coordinates " << selected.to_string();
    v.message(sstream.str());

    return selected;
}

unsigned Solver::select_val(Coordinates c) const{
    unsigned max = 0;
    unsigned selected_val = 0;

    std::set<unsigned>& available_vals = grid.get_available_vals(c);

    if (available_vals.size() == 1){
        selected_val = *available_vals.begin();
        std::stringstream sstream;
        sstream << "Selected value " << selected_val << " as it is the only one possible";
        v.message(sstream.str());
        return selected_val;
    }

    for(auto iter = available_vals.begin(); iter != available_vals.end(); ++iter){
        unsigned min_constraints_num = grid.simulate_add_constraint(c.get_x(), c.get_y(), *iter);

        if (min_constraints_num >= max){
            max = min_constraints_num;
            selected_val = *iter;
        }
    }

    // TODO: Create and push a new checkpoint. Remove the current value from available values in the checkpoint
    // Store in the checkpoint the current iterator
    // so that we can restore this method from the subsequent value.
    // If there are no more values available, pop another checkpoint

    std::stringstream sstream;
    sstream << "Selected value " << selected_val << " as it is the less constraining value";
    v.message(sstream.str());

    return selected_val;
}

void Solver::solve() const{
    while (!grid.is_completed()){
        Coordinates c = select_cell();
        unsigned val = select_val(c);
        grid.set(c, val);
        v.draw();

        std::string s;
        std::cin >> s;
    }
}